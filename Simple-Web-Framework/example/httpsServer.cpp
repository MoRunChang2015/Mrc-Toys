#include "httpsServer.hpp"
#include "handler.hpp"

int main() {
    SimpleWebServer::Server<SimpleWebServer::HTTPS> server(
        8080, 4, "example/ssl/server.crt", "example/ssl/server.key");
    std::cout << "Server starting at port: " << 8080 << std::endl;
    start_server<SimpleWebServer::Server<SimpleWebServer::HTTPS>>(server);
    return 0;
}
