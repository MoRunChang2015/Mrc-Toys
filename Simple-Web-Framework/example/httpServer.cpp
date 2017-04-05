#include "httpServer.hpp"
#include "handler.hpp"

int main() {
    // http server running at port 8000 with 4 threads
    SimpleWebServer::Server<SimpleWebServer::HTTP> server(8080, 4);
    std::cout << "Server starting at port: " << 8080 << std::endl;
    start_server<SimpleWebServer::Server<SimpleWebServer::HTTP>>(server);
    return 0;
}
