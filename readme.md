MoRunChang toys
===========

This repository contains MoRunChang's toys for black magic codes or experiments:

* Web Server Farmework



### Web Server Farmework

A very simple, multithreaded HTTP and HTTPS web server framework implemented using C++11 and Boost.Asio.

usage:
```c++
    SimpleWebServer::Server<SimpleWebServer::HTTP> server(PORT, NUMBER_OF_THREAD);  // or HTTPS


    server.resource["/regex/"]["method"] = [](std::ostream& response, Resquest& request) {
        
        // response function
        
    }


    server.start()

```






