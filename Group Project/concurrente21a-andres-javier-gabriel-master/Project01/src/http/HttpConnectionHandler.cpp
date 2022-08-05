/// Copyright 2021 Javier Molina, Gabriel Zúñiga and Andrés Azofeifa.
/// Universidad de Costa Rica
/// Handles all the requests of a client

#include "HttpConnectionHandler.hpp"

#include <unistd.h>
#include <iostream>

HttpConnectionHandler::HttpConnectionHandler
(HttpServer* the_server, size_t thread_index) {
  http_server = the_server;
  thread_id = thread_index;
}

HttpConnectionHandler::~HttpConnectionHandler() {
}

/**
 * @brief This method handles all the http requests of the 
 * given Socket, it does not end until all requests are done.
 * 
 * @param data The Socket that act as a client
 */
void HttpConnectionHandler::consume(const Socket& data) {
  Socket temporalSocket = data;
  while (true) {
    // Create an object that parses the HTTP request from the socket
    HttpRequest httpRequest(temporalSocket);
    // sleep(20);
    // std::cout<<"Thread "<<thread_id<<" Handling a connection";
    // If the request is not valid or an error happened
    if (!httpRequest.parse()) {
      // Non-valid requests are normal after a previous valid request. Do not
      // close the connection yet, because the valid request may take time to
      // be processed. Just stop waiting for more requests
      break;
    }

    // A complete HTTP client request was received. Create an object for the
    // server responds to that client's request
    HttpResponse httpResponse(temporalSocket);

    // Give subclass a chance to respond the HTTP request
    const bool handled = this->http_server->
    handleHttpRequest(httpRequest, httpResponse);

    // If subclass did not handle the request or the client used HTTP/1.0
    if (!handled || httpRequest.getHttpVersion() == "HTTP/1.0") {
      // The socket will not be more used, close the connection
      temporalSocket.close();
      break;
    }

    // This version handles just one client request per connection
    // TODO(you): Remove this break after parallelizing this method
    break;
  }
}

/**
 * @brief calls the method consumeForever(). 
 * This is the subroutine that the threads will run, is 
 * an override of the virtual run() of the Thread class.
 * @return int 
 */
int HttpConnectionHandler::run() {
  this->consumeForever();
  return 0;
}
