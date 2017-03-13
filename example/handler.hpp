#include <fstream>
#include <sstream>
#include "baseServer.hpp"

template <typename SERVER>
void start_server(SERVER& server) {
    // handler "/string" POST Request
    server.resource["^/string/?$"]["POST"] = [](
        ostream& response, SimpleWebServer::Request& request) {
        std::stringstream ss;
        *request.content >> ss.rdbuf();
        std::string content = ss.str();

        response << "HTTP/1.1 200 OK \r\nContent-Length: " << content.length()
                 << "\r\n\r\n"
                 << content;
    };

    // handler "/info" GET Request
    server.resource["^/info/?$"]["GET"] = [](ostream& response,
                                             Request& request) {
        stringstream content_stream;
        content_stream << "<h1>Request:</h1>";
        content_stream << request.method << " " << request.path << " HTTP/"
                       << request.http_version << "<br>";
        for (auto& header : request.header) {
            content_stream << header.first << ": " << header.second << "<br>";
        }

        content_stream.seekp(0, ios::end);
        // content_stream.tellp() is the length of content
        response << "HTTP/1.1 200 OK\r\nContent-Length: "
                 << content_stream.tellp() << "\r\n\r\n"
                 << content_stream.rdbuf();
    };

    // default Response
    server.default_resource["^/?(.*)$"]["GET"] = [](ostream& response,
                                                    Request& request) {
        std::string filename = "example/static/";

        std::string path = request.path_match[1];

        size_t last_pos = path.rfind(".");
        size_t current_pos = 0;
        size_t pos;

        // Prevent to access the "static/" directory outside the content using
        // ".."
        while ((pos = path.find('.', current_pos)) != string::npos &&
               pos != last_pos) {
            current_pos = pos;
            path.erase(pos, 1);
            last_pos--;
        }

        filename += path;
        std::ifstream ifs;
        if (filename.find('.') == string::npos) {
            if (filename[filename.length() - 1] != '/') filename += '/';
            filename += "index.html";
        }
        ifs.open(filename, ifstream::in);

        if (ifs) {
            ifs.seekg(0, ios::end);
            size_t length = ifs.tellg();

            ifs.seekg(0, ios::beg);

            response << "HTTP/1.1 200 OK\r\nContent-Length: " << length
                     << "\r\n\r\n"
                     << ifs.rdbuf();

            ifs.close();
        } else {
            string content = "Could not open file " + filename;
            response << "HTTP/1.1 400 Bad Request\r\nContent-Length: "
                     << content.length() << "\r\n\r\n"
                     << content;
        }
    };

    // start the server
    server.start();
}
