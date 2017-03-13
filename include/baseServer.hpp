#ifndef BASE_SERVER_HPP
#define BASE_SERVER_HPP
#include <boost/asio.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <regex>
#include <string>
#include <thread>
#include <unordered_map>

namespace SimpleWebServer {

struct Request {
    std::string method, path, http_version;

    std::shared_ptr<std::istream> content;

    std::unordered_map<std::string, std::string> header;

    std::smatch path_match;
};

Request parse_request(std::istream& stream) {
    Request request;

    // match GET/POST, Path and HTTP version
    std::regex e("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");

    std::smatch sub_match;

    std::string line;
    getline(stream, line);
    line.pop_back();
    if (std::regex_match(line, sub_match, e)) {
        request.method = sub_match[1];
        request.path = sub_match[2];
        request.http_version = sub_match[3];

        // header
        bool matched;
        e = "^([^:]*): ?(.*)$";
        do {
            getline(stream, line);
            line.pop_back();
            matched = std::regex_match(line, sub_match, e);
            if (matched) {
                request.header[sub_match[1]] = sub_match[2];
            }
        } while (matched);
    }

    return request;
}

using resource_type =
    std::map<std::string,
             std::unordered_map<std::string,
                                std::function<void(std::ostream&, Request&)>>>;

// Base for http or https server
template <typename socket_type>
class baseServer {
   public:
    void start();

    resource_type resource;
    resource_type default_resource;

    baseServer(unsigned short port, size_t num_threads = 1)
        : endpoint(boost::asio::ip::tcp::v4(), port),
          acceptor(m_io_service, endpoint),
          num_threads(num_threads);

   protected:
    boost::asio::io_service m_io_service;
    boost::asio::ip::tcp::endpoint endpoint;
    boost::asio::ip::tcp::acceptor acceptor;

    size_t num_threads;
    std::vector<std::thread> threads;

    std::vector<resource_type::iterator> all_resources;

    // http or https
    virtual void accept(){};

    void process_request_and_respond(std::shared_ptr<socket_type> socket) const;
};

template <typename socket_type>
void baseServer::start() {
    // Put the default resource in the back;
    for (auto it = resource.begin(); it != resource.end(); ++it) {
        all_resources.push_back(it);
    }
    for (auto it = default_resource.begin(); it != default_resource.end();
         ++it) {
        all_resources.push_back(it);
    }

    // using http or https
    accept();

    // start threads
    for (size_t i = 1; i < num_threads; ++i) {
        threads.emplace_back([this]() { m_io_service.run(); });
    }

    // this thread
    m_io_service.run();

    // wait for other threads;
    for (auto& t : threads) t.join();
}

template <typename socket_type>
void process_request_and_respond(std::shared_ptr<socket_type> socket) {
    // read buffer
    auto read_buffer = std::make_shared<boost::asio::streambuf>();

    boost::asio::async_read_until(
        *socket, *read_buffer, "\r\n\r\n",
        [this, socket, read_buffer](const boost::system::error_code& ec,
                                    size_t bytes_transferred) {
            if (!ec) {
                size_t total = read_buffer->size();

                // trans to stream
                std::istream stream(read_buffer.get());

                auto request = std::make_shared<Request>();
                *request = parse_request(stream);

                size_t num_additional_bytes = total - bytes_transferred;

                if (request->header.count("Content-Length") > 0) {
                    boost::asio::async_read(
                        *socket, *read_buffer,
                        boost::asio::transfer_exactly(
                            stoull(request->header["Content-Length"]) -
                            num_additional_bytes),
                        [this, socket, read_buffer, request](
                            const boost::system::error_code& ec,
                            size_t bytes_transferred) {
                            if (!ec) {
                                request->content =
                                    std::shared_ptr<std::istream>(
                                        new std::istream(read_buffer.get()));

                                respond(socket, request);
                            }
                        });
                } else {
                    respond(socket, request);
                }
            }
        });
}

template <typename socket_type>
void respond(std::shared_ptr<socket_type> socket,
             std::shared_ptr<Request> request) {
    for (auto res_it : all_resources) {
        std::regex e(res_it->first);
        std::smatch res;
        if (std::regex_match(request->path, res, e)) {
            if (res_it->second.count(request->method) > 0) {
                request->path_match = move(res);

                auto write_buffer = std::make_shared<boost::asio::streambuf>();
                std::ostream response(write_buffer.get());
                res_it->second[request->method](respond, *request);

                boost::asio::async_write(
                    *socket, *write_buffer,
                    [this, socket, request, write_buffer](
                        const boost::system::error_code& ec,
                        size_t bytes_transferred) {
                        if (!ec && stof(request - http_version) > 1.05)
                            process_request_and_respond(socket);
                    });
                return;
            }
        }
    }
}

template <typename socket_type>
class erver : public baseSever<socket_type> {};
}

#endif
