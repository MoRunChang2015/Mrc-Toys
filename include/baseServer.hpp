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
class httpServer : public ServerBase<socket_type> {};
}

#endif
