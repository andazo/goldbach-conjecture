// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#include <cassert>
#include <stdexcept>
#include <string>

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "HttpServer.hpp"
#include "Socket.hpp"
#include "HttpConnectionHandler.hpp"

HttpServer::HttpServer() {
  connectionsQueue = new Queue<Socket>();
}

/**
 * @brief returns the atribute max_connections 
 * 
 * @return size_t max_connections
 */
size_t HttpServer::get_max_connections() {
  return max_connections;
}

/**
 * @brief /// sets the max number of connections that the httpserver accept
 * 
 * @param max_number_of_connections The new max number of connections
 */
void HttpServer::set_max_connections(size_t max_number_of_connections) {
  max_connections = max_number_of_connections;
}

std::vector<HttpConnectionHandler*> HttpServer::getConnectionWorkers() {
  return connectionWorkers;
}

/**
 * @brief /// Returns the atribute connectionQueue of the httpserver
 * 
 * @return Queue<Socket>* A pointer to the queue atribute
 */
Queue<Socket>* HttpServer::get_connections_queue() {
  return connectionsQueue;
}

HttpServer::~HttpServer() {
}

void HttpServer::listenForever(const char* port) {
  return TcpServer::listenForever(port);
}

void HttpServer::handleClientConnection(Socket& client) {
  connectionsQueue->push(client);
}

/**
 * @brief Creates the HttpConnectionHandler objects based on the max numbers
 * of connections that the httpserver accept, then store this objects in the 
 * connectionWorkers atribute of this class.
 * 
 */
void HttpServer::create_workers() {
  Queue<goldbachSums>* goldbachWorkQueue = new Queue<goldbachSums>();
  for (size_t index = 0; index < max_connections; index++) {
    HttpConnectionHandler *worker = new HttpConnectionHandler(this, index);
    worker->setProducingQueue(goldbachWorkQueue);
    worker->setConsumingQueue(connectionsQueue);
    worker->startThread();
    connectionWorkers.push_back(worker);
  }
}

/**
 * @brief Waits all the threads to finish they works.
 * 
 */
void HttpServer::join_workers() {
  for (size_t index = 0; index < max_connections; index++) {
    connectionWorkers[index]->waitToFinish();
    delete(connectionWorkers[index]);
  }
}
