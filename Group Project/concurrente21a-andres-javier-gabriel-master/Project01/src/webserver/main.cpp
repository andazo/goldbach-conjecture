// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0
// Serial web server's initial code for parallelization

#ifdef WEBSERVER

#include <signal.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>

#include "WebServer.hpp"

static void signal_handler(int numSignal) {
  char response;
  switch (numSignal) {
    case SIGINT:
      printf(" Do you wish to shutdown the server? [y/n] ");
      std::cin >> response;
      if (response == 'y' || response == 'Y') {
        std::cout << "\nReceived signal " << numSignal << " SIGINT\n";
        WebServer::getInstance().stop();
      }
      break;
    case SIGTERM:
      std::cout << "\nReceived signal " << numSignal << " SIGTERM\n";
      WebServer::getInstance().stop();
    break;
  }
}

int main(int argc, char* argv[]) {
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);
  int error = WebServer::getInstance().start(argc, argv);
  return error;
}

#endif  // WEBSERVER
