#ifndef HTTPS_SERVER_HPP
#define HTTPS_SERVER_HPP
#include <boost/asio/ssl.hpp>
#include "baseServer.hpp"

namespace SimpleWebServer {
using HTTPS = boost::asio::ssl::stream<boost::asio::ip::tcp::socket>;

template <>
class Server<HTTPS> : public baseServer<HTTPS> {
   public:
    Server(unsigned short port, size_t num_threads,
           const std::string& cert_file, const std::string& private_key_file)
        : baseServer<HTTPS>::baseServer(port, num_threads),
          context(boost::asio::ssl::context::sslv23) {
        context.use_certificate_chain_file(cert_file);
        context.use_private_key_file(private_key_file,
                                     boost::asio::ssl::context::pem);
    }

   private:
    boost::asio::ssl::context context;

    void accept() {
        // New a socket with ssl context
        auto socket = std::make_shared<HTTPS>(m_io_service, context);

        acceptor.async_accept(
            (*socket).lowest_layer(),
            [this, socket](const boost::system::error_code& ec) {
                accept();

                if (!ec) {
                    (*socket).async_handshake(
                        boost::asio::ssl::stream_base::server,
                        [this, socket](const boost::system::error_code& ec) {
                            if (!ec) process_request_and_respond(socket);
                        });
                }
            });
    }
};
}

#endif
