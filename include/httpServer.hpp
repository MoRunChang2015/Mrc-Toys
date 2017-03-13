#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP
#include "baseServer.hpp"

namespace SimpleWebServer {
using HTTP = boost::asio::ip::tcp::socket;
template <>
class Server<HTTP> : public baseServer<HTTP> {
   public:
    Server(unsigned short port, size_t num_threads = 1)
        : baseServer<HTTP>::baseServer(port, num_threads){};

   private:
    void accept() {
        auto socket = std::make_shared<HTTP>(m_io_service);

        acceptor.async_accept(
            *socket, [this, socket](const boost::system::error_code& ec) {
                accept();
                if (!ec) process_request_and_respond(socket);
            });
    }
};
}

#endif
