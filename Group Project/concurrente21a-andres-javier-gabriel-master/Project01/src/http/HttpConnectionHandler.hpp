/// Copyright 2021 Javier Molina, Gabriel Zúñiga and Andrés Azofeifa.
/// Universidad de Costa Rica
/// Handles all the requests of a client


#include <vector>
#include "Assembler.hpp"
#include "HttpServer.hpp"
#include "Socket.hpp"
#include "GoldbachCalculator.hpp"

#ifndef HTTPCONNECTIONHANDLER_HPP

/**
 * @brief This class is inherits from the Consumer class, that 
 * is a child of the Thread class then, every instance of this 
 * class is a Thread that consume from a queue of Sockets.  
 * When a Socket is consume all of their requests to the server
 * are handled.
 */
class HttpConnectionHandler : public Assembler<Socket, goldbachSums>{
 public :
  HttpConnectionHandler(HttpServer* the_server, size_t thread_id);
  ~HttpConnectionHandler();
  ///  The server to work with
 private:
  HttpServer* http_server;
  ///  For debbuging
  size_t thread_id;

 protected:
  void consume(const Socket& data) override;
  int run() override;
};

#endif  // HTTPCONNECTIONHANDLER_HPP
