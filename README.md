# Simple-Web-Framework
A very simple, multithreaded HTTP and HTTPS web server framework implemented using C++11 and Boost.Asio. 

### Usage

See [httpServer.cpp](https://github.com/MoRunChang2015/Simple-Web-Framework/blob/master/example/httpServer.cpp) or [httpsServer.cpp](https://github.com/MoRunChang2015/Simple-Web-Framework/blob/master/example/httpServer.cpp)

### Complie and run the example

+ make httpServer and httpsServer
```bash
make
```
+ make httpServer and run
```bash
make http
bin/httpServer
```
+ For https

> + Create a certificate and private key (in example/ssl/)
```bash
openssl genrsa -des3 -out server.key 1024
openssl req -new -key server.key -out server.csr
cp server.key server.key.bk
openssl rsa -in server.key.bk -out server.key
openssl x509 -req -days 365 -in server.csr -signkey server.key -out server.crt
```

> + make httpsServer and run
```
make https
bin/httpsServer
```


### Dependencies
+ g++ (version >= 4.9)
+ Boost C++ libraries
+ OpenSSl libraries


### TODO list

+ Exception handling

+ Log

+ Thread pool？
