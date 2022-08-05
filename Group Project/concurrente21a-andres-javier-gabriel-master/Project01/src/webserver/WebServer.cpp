// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#include <unistd.h>
#include <cassert>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include "WebServer.hpp"
#include "NetworkAddress.hpp"
#include "Socket.hpp"
#include "GoldbachWebApp.hpp"

const char* const usage =
  "Usage: webserv [port] [max_connections]\n"
  "\n"
  "  port             Network port to listen incoming HTTP requests, default "
    DEFAULT_PORT "\n"
  "  max_connections  Maximum number of allowed client connections\n";
const size_t DEFAULT_MAX_CONNECTIONS = 100;

///  Constructor
WebServer::WebServer() {
  didStop = false;
}

///  Destructor
WebServer::~WebServer() {
}

/**
 * @brief Returns the existent instance of the class,
 * since it's a singleton class.
 * 
 */
WebServer& WebServer::getInstance() {
  static WebServer instance;
  return instance;
}

/**
 * @brief Stops receiving connections so no new users can enter, and stops the
 * server entirely when the pending requests are processed.
 * 
 */
void WebServer::stop() {
  this->stopListening();
  didStop = true;
  for (int64_t index = 0; index < sysconf(_SC_NPROCESSORS_ONLN); index++) {
    goldbachSums* empty = new goldbachSums();  // Socket object
    this->getConnectionWorkers()[0]->getProducingQueue()->push(*empty);
    delete empty;
  }
  this->goldbachWebApp.joinGoldbachWorkers();
  delete(this->getConnectionWorkers()[0]->getProducingQueue());
  for (size_t i = 0; i < this->get_max_connections(); i++) {
    Socket socket;  // Socket object
    this->get_connections_queue()->push(socket);
  }
  this->join_workers();
  delete(this->get_connections_queue());
  std::cout << "Server terminated\n";
}

/**
 * @brief Starts the server and creates the threads for both the connections
 * and the calculators.
 */
int WebServer::start(int argc, char* argv[]) {
  try {
    if (this->analyzeArguments(argc, argv)) {
      // void stop();
      // TODO(you) Handle signal 2 (SIGINT) and 15 (SIGTERM), see man signal
      // Signal handler should call WebServer::stopListening(), send stop
      // conditions and wait for all secondary threads that it created
      this->create_workers();
      goldbachWebApp.setGoldbachWorkQueue
      (this->getConnectionWorkers()[0]->getProducingQueue());
      goldbachWebApp.createGoldbachWorkers();
      this->listenForConnections(this->port);
      const NetworkAddress& address = this->getNetworkAddress();
      std::cout << "web server listening on " << address.getIP()
        << " port " << address.getPort() << "...\n";
      this->acceptAllConnections();
    }
  } catch (const std::runtime_error& error) {
    if (!didStop) {
      std::cerr << "error: " << error.what() << std::endl;
    } else {
    }
  }
  return EXIT_SUCCESS;
}

/**
 * @brief Analizes the arguments and sets the port to be used and the number
 * of max connections if they were specified, if not they are set to a default
 * value.
 */
bool WebServer::analyzeArguments(int argc, char* argv[]) {
  size_t max_connections = DEFAULT_MAX_CONNECTIONS;
  // Traverse all arguments
  for (int index = 1; index < argc; ++index) {
    const std::string argument = argv[index];
    if (argument.find("help") != std::string::npos) {
      std::cout << usage;
      return false;
    }
  }

  if (argc >= 2) {
    this->port = argv[1];
  }
  // Assings the max_connections atribute, this is the number of threads that
  // will manage connections.
  if (argc >= 3) {
    if (sscanf(argv[2], "%zu", &max_connections) < 1
    || max_connections == 0) {
      std::cout<< "error: invalid max conection cout\n";
    }
  }
  this->set_max_connections(max_connections);
  return true;
}

/**
 * @brief Prints information about the users in the server console and sends
 * them to the route method.
 */
bool WebServer::handleHttpRequest(HttpRequest& httpRequest,
    HttpResponse& httpResponse) {
  // Print IP and port from client
  const NetworkAddress& address = httpRequest.getNetworkAddress();
  std::cout << "connection established with client " << address.getIP()
  << " port " << address.getPort() << std::endl;
  // Print HTTP request
  std::cout << "  " << httpRequest.getMethod()
  << ' ' << httpRequest.getURI()
  << ' ' << httpRequest.getHttpVersion() << std::endl;
  return this->route(httpRequest, httpResponse);
}

/**
 * @brief Shows the user the page they asked for.
 * 
 */
bool WebServer::route(HttpRequest& httpRequest, HttpResponse& httpResponse) {
  // If the home page was asked
  if (httpRequest.getMethod() == "GET" && httpRequest.getURI() == "/") {
      // Set HTTP response metadata (headers)
    httpResponse.setHeader("Server", "AttoServer v1.0");
    httpResponse.setHeader("Content-type", "text/html; charset=ascii");
    return goldbachWebApp.serveHomepage(httpRequest, httpResponse);
  }

  if (httpRequest.getMethod() == "GET" && httpRequest.getURI() == "/help") {
      // Set HTTP response metadata (headers)
    httpResponse.setHeader("Server", "AttoServer v1.0");
    httpResponse.setHeader("Content-type", "text/html; charset=ascii");
    return goldbachWebApp.serveHelp(httpRequest, httpResponse);
  }

  std::smatch matches;

  // If a number was asked in the form "/goldbach/1223"
  // or "/goldbach?number=1223"
  // std::regex inQuery("^/goldbach(/|\\?number=)-?(\\d*(\\.\\d+)?)$");
  // ^\\d+(,\d+)* positivos [+-]?\\d+(,[+-]?\\d+)*

  // Support for directly writing in the url: /goldbach?number=12,32...
  std::string URI = httpRequest.getURI();
  for (size_t index = 0; index < URI.length(); index++) {
    if (URI[index] == ',') {
      URI.replace(index, 1, "%2C");
    }
  }

  // Support for the input bar in Homepage
  std::regex inQuery("^/goldbach(/|\\?number=)([+-]?\\d+(%2C[+-]?\\d+)*)$");
  if (std::regex_search(URI, matches, inQuery)) {
    assert(matches.length() >= 3);
    std::vector<int64_t> numbers;  // The numbers are saved here
    std::string data = matches[2];  // Copies the input into a string
    std::string delimiter = "%2C";  // Separator of each number in the string
    size_t pos = 0;  // Marks each number's end in the string
    std::string token = "";  // Saves the number after separating it
                             // from the string
    while ((pos = data.find(delimiter)) != std::string::npos) {
      token = data.substr(0, pos);
      try {  // Won't accept numbers larger than 9,223,372,036,854,775,807
        // but on the negative side can still take -9,223,372,036,854,775,808
        if (token != OUT_OF_RANGE_NEGATIVE_VALUE) {  // Ignore it too
          numbers.push_back(std::stoll(token));
        }
      } catch (...) {}
      data.erase(0, pos + delimiter.length());  // Actualiza el string
    }
    try {  // Won't accept numbers larger than 9,223,372,036,854,775,807
      // but on the negative side can still take -9,223,372,036,854,775,808
      if (data != OUT_OF_RANGE_NEGATIVE_VALUE) {  // Ignore it too
        numbers.push_back(std::stoll(data));
      }
    } catch (...) {}
    if (numbers.empty()) {
      // this->mutex.unlock();
      return goldbachWebApp.serveOutOfRange(httpRequest, httpResponse);
    }
    // this->mutex.unlock();

    return goldbachWebApp.serveGoldbachSums(httpRequest, httpResponse, numbers);
  }
  // Unrecognized request
  return goldbachWebApp.serveNotFound(httpRequest, httpResponse);
}
