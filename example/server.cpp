#include "httpServer.hpp"
#include "handler.hpp"

int main() {
    // http server running at port 8000 with 4 threads
    SimpleWebServer::Server<HTTP> server(8000, 4);
    start_server<Server<HTTP>>(server);
    return 0;
}
