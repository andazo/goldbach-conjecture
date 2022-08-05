// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <vector>

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "TcpServer.hpp"
#include "Queue.hpp"


class HttpConnectionHandler;

class HttpServer : public TcpServer {
  DISABLE_COPY(HttpServer);

 protected:
  /// Lookup criteria for searching network information about this host
  struct addrinfo hints;

 public:
  /// Constructor
  HttpServer();
  /// Destructor
  ~HttpServer();
  /// Infinetelly listen for client connection requests and accept all of them.
  /// For each accepted connection request, the virtual onConnectionAccepted()
  /// will be called. Inherited classes must override that method
  void listenForever(const char* port);
  virtual bool handleHttpRequest(HttpRequest& httpRequest,
  HttpResponse& httpResponse) = 0;
  /// returns the atribute max_connections
  size_t get_max_connections();
  /// sets the max number of connections of the httpserver
  void set_max_connections(size_t max_number_of_connections);
  /// Returns the atribute connectionQueue of the httpserver
  Queue<Socket>* get_connections_queue();
  std::vector<HttpConnectionHandler*> getConnectionWorkers();
  void create_workers();
  void join_workers();

 private:
  ///  The max number of connections that the server can accept
  size_t max_connections;
  ///  The queue of accepted connections
  Queue<Socket> *connectionsQueue;
  ///  Vector of objets HttpConnectionHandler that act as a threads
  std::vector<HttpConnectionHandler*> connectionWorkers;

 protected:
  /// This method is called each time a client connection request is accepted.
  void handleClientConnection(Socket& client) override;
  /// Called each time an HTTP request is received. Web server should analyze
  /// the request object and assemble a response with the response object.
  /// Finally send the response calling the httpResponse.send() method.
  /// @return true on success and the server will continue handling further
  /// HTTP requests, or false if server should stop accepting requests from
  /// this client (e.g: HTTP/1.0)
};

#endif  // HTTPSERVER_H
