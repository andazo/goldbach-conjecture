/// Copyright 2021 Javier Molina, Grabriel Zúñiga and Andrés Azofeifa.
/// Universidad de Costa Rica.
/// View Class for the WebServer Class.

#ifndef GOLDBACHWEBAPP_H
#define GOLDBACHWEBAPP_H

#include <inttypes.h>
#include <vector>
#include "HttpServer.hpp"
#include "GoldbachCalculator.hpp"

/**
 * @brief This class is responsible for displaying the graphical interface
 * in the web browser and answering to the user.
 * 
 */
  class GoldbachWebApp  {
   public:
    ///  Constructor
    GoldbachWebApp();
    ///  Destructor
    ~GoldbachWebApp();
    /// Sends the homepage as HTTP response
    bool serveHomepage(HttpRequest& httpRequest, HttpResponse& httpResponse);
    /// Sends the help page as HTTP response
    bool serveHelp(HttpRequest& httpRequest, HttpResponse& httpResponse);
    /// Sends a page for a non found resouce in this server
    bool serveNotFound(HttpRequest& httpRequest, HttpResponse& httpResponse);
    /// Sends a page for an out of range input if there's only one number
    bool serveOutOfRange(HttpRequest& httpRequest, HttpResponse& httpResponse);
    /// Calculate the prime factorization of a number asked by the client, and
    /// sends the response in HTML format as HTTP response
    bool serveGoldbachSums(HttpRequest& httpRequest
    , HttpResponse& httpResponse, std::vector<int64_t> numbers);
    void createGoldbachWorkers();
    void joinGoldbachWorkers();
    size_t getMaxGoldbachWorkers();
    void setGoldbachWorkQueue(Queue<goldbachSums>* queue);
    Queue<goldbachSums>* getQueue();
    bool fillSums(goldbachSums* responseArray, HttpResponse& httpResponse,
    size_t numbersLenght);

   private:
    std::vector<GoldbachCalculator*> goldbachWorkers;
    size_t maxGoldbachWorkers;
    Queue<goldbachSums>* goldbachWorkQueue;
  };

#endif  // GOLDBACHWEBAPP_H
