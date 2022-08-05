// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <mutex>
#include <vector>
#include <string>

#include "GoldbachWebApp.hpp"
#include "HttpConnectionHandler.hpp"
#include "HttpServer.hpp"

#define DEFAULT_PORT "8080"

const char OUT_OF_RANGE_NEGATIVE_VALUE[] = "-9223372036854775808";

/**
 * @brief This class starts and stops the server, and works as a bridge in
 * between the web pages.
 * 
 */
class WebServer : public HttpServer {
  DISABLE_COPY(WebServer);

 private:
  /// TCP port where this web server will listen for connections
  const char* port = DEFAULT_PORT;
  ///  The instance for singleton class
  static WebServer instance;
  /// Constructor
  WebServer();

 public:
  /// Destructor
  ~WebServer();
  ///  Starts the WebServer
  int start(int argc, char* argv[]);
  ///  Stops the web server
  void stop();
  ///  Returns the instance of WebServer
  static WebServer& getInstance();
  ///  Object of the class GoldbachWebApp that shows the grafical interfaces
  GoldbachWebApp goldbachWebApp;

  ///  Boolean that represents wether the server is stoped or not
  bool didStop;

 protected:
  /// Analyze the command line arguments
  /// @return true if program can continue execution, false otherwise
  bool analyzeArguments(int argc, char* argv[]);
  /// Handle HTTP requests. @see HttpServer::handleHttpRequest()
  bool handleHttpRequest(HttpRequest& httpRequest,
    HttpResponse& httpResponse) override;

 protected:
  /// Route, that provide an answer according to the URI value
  /// For example, home page is handled different than a number
  bool route(HttpRequest& httpRequest, HttpResponse& httpResponse);
};

#endif  // WEBSERVER_H
