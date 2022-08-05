/// Copyright 2021 Javier Molina, Grabriel Zúñiga and Andrés Azofeifa.
/// Universidad de Costa Rica.
/// View Class for the WebServer Class.

#include <unistd.h>
#include <string>
#include <iostream>
#include "GoldbachWebApp.hpp"

///  Constructor
GoldbachWebApp::GoldbachWebApp() {
}
///  Destructor
GoldbachWebApp::~GoldbachWebApp() {
}

/**
 * @brief Returns the attribute maxGoldbachWorkers
 * 
 * @return size_t 
 */
size_t GoldbachWebApp::getMaxGoldbachWorkers() {
  return maxGoldbachWorkers;
}

/**
 * @brief Returns the attribute goldbachWorkQueue
 * 
 * @return Queue<goldbachSums>* 
 */
Queue<goldbachSums>* GoldbachWebApp::getQueue() {
  return goldbachWorkQueue;
}

/**
 * @brief Set the goldbachWorkQueue object
 * 
 * @param queue 
 */
void GoldbachWebApp:: setGoldbachWorkQueue(Queue<goldbachSums>* queue) {
  goldbachWorkQueue = queue;
}

/**
 * @brief Creates the goldbachWorkes necessary to calculate goldbach sums
 * 
 */
void GoldbachWebApp::createGoldbachWorkers() {
  maxGoldbachWorkers = sysconf(_SC_NPROCESSORS_ONLN);
  for (size_t index = 0; index < maxGoldbachWorkers; index++) {
    GoldbachCalculator *worker = new GoldbachCalculator(index);
    worker->setConsumingQueue(goldbachWorkQueue);
    worker->startThread();
    goldbachWorkers.push_back(worker);
  }
}

/**
 * @brief wait until the goldbachWorkers that are threads, finish they jobs
 * 
 */
void GoldbachWebApp::joinGoldbachWorkers() {
  for (size_t index = 0; index < maxGoldbachWorkers; index++) {
    goldbachWorkers[index]->waitToFinish();
    delete(goldbachWorkers[index]);
  }
}

/**
 * @brief Shows the homepage of the server.
 * @return httpResponse Contains the data the browser needs to show the page.
 */

bool GoldbachWebApp::serveHomepage(HttpRequest& httpRequest
  , HttpResponse& httpResponse) {
  (void)httpRequest;

  // Build the body of the response
  std::string title = "Goldbach sums";  // Page title
  httpResponse.body() << "<!DOCTYPE html>\n"
    << "<html lang=\"en\">\n"
    << "  <meta charset=\"ascii\"/>\n"
    << "  <meta name=\"viewport\" content=\"width=device-width,"
    << "    initial-scale=1.0\">\n"
    << "  <title>" << title << "</title>\n"
    << "  <style>\n"  // Page configuration
    << "    body { font-family: monospace;\n"
    << "      background-color:black;\n"
    << "      text-align:center;\n"
    << "      color:white; }\n"
    << "    p { font-size: 20px; }\n"
    << "    a { color:white; }\n"
    << "    h1 { font-size: 32px; }\n"
    << "    hr { color: white; }\n"
    << "    label { font-size: 20px; }\n"
    << "    input { font-size: 20px; }\n"
    << "    button { font-size: 20px; }\n"
    << "    .center {\n"  // Used in Div to center page content
    << "      margin: 0;\n"
    << "      position: absolute;\n"
    << "      top: 50%;\n"
    << "      left: 50%;\n"
    << "      transform: translate(-50%, -50%); }\n"
    << "    .header {\n"  // Copyright footer
    << "      position: fixed;\n"
    << "      left: 0;\n"
    << "      top: 0;\n"
    << "      width: 99%;\n"
    << "      background-color: black;\n"
    << "      color: white;\n"
    << "      text-align: right; }\n"
    << "    .footer {\n"  // Copyright footer
    << "      position: fixed;\n"
    << "      left: 0;\n"
    << "      bottom: 0;\n"
    << "      width: 100%;\n"
    << "      background-color: black;\n"
    << "      color: white;\n"
    << "      text-align: center; }\n"
    << "  </style>\n"
    << "  <div class=\"header\">\n"  // Footer
    << "    <p><a href=\"/help\">Help</a></p>\n"  // Help button
    << "  </div>\n"
    << "  <div class=\"center\">\n"  // Page content
    << "    <h1>" << title << "</h1>\n"  // Title
    << "    <hr>\n"  // Separator (line in between title and message)
    << "    <p>Introduce one or more numbers, if more than one,"
    << "      separated by \",\"</p>\n"  // Message
    << "    <p>Example: 13,-21,8</p>\n"  // Message
    << "    <br>\n"  // Blank line
    << "    <form method=\"get\" action=\"/goldbach\">\n"
    << "      <label for=\"number\">Number(s)</label>\n"  // Label
    << "      <input type=\"text\" name=\"number\" required/>\n"  // Input
    << "      <button type=\"submit\">Calculate</button>\n"  // Button
    << "    </form>\n"
    << "  </div>\n"
    << "  <div class=\"footer\">\n"  // Footer
    << "    <p>Copyright: Los Concurrentes</p>\n"  // Message
    << "  </div>\n"
    << "</html>\n";

  // Send the response to the client (user agent)
  return httpResponse.send();
}

/**
 * @brief Shows the Help page if user press the help button.
 * @return httpResponse Contains the data the browser needs to show the page.
 */

bool GoldbachWebApp::serveHelp(HttpRequest& httpRequest
  , HttpResponse& httpResponse) {
  (void)httpRequest;

  // Build the body of the response
  std::string title = "Help";  // Page title
  httpResponse.body() << "<!DOCTYPE html>\n"
    << "<html lang=\"en\">\n"
    << "  <meta charset=\"ascii\"/>\n"
    << "  <meta name=\"viewport\" content=\"width=device-width,"
    << "    initial-scale=1.0\">\n"
    << "  <title>" << title << "</title>\n"
    << "  <style>\n"  // Page configuration
    << "    body { font-family: monospace;\n"
    << "      background-color:black; }\n"
    << "    h1 { font-size: 28px;\n"
    << "      color:white; }\n"
    << "    p { color:white;\n"
    << "    text-align: justify;\n"
    << "    font-size: 20px;\n"
    << "    padding: 0 2%; }\n"
    << "    ol { color:white; }\n"
    << "    a { color:white; }\n"
    << "    .centerAll {\n"  // Used in Div to center page content
    << "      position: absolute;\n"
    << "      top: 0;\n"
    << "      left: 0;\n"
    << "      right: 0;\n"
    << "      bottom: 0;\n"
    << "      display: flex;\n"
    << "      flex-direction: column; }\n"
    << "    .centerTitle {\n"  // Used in Div to center page title
    << "      margin: auto;\n"
    << "      text-align:center; }\n"
    << "    .imgbox {"
    << "      display: grid;"
    << "      height: 100%; }"
    << "    .centerFit {"
    << "      max-width: 100%;"
    << "      max-height: 100vh;"
    << "      margin: auto; }"
    << "  </style>\n"
    << "  <div class=\"centerAll\">"  // Page content
    << "    <div class=\"centerTitle\">"
    << "      <h1>What are Goldbach sums?</h1>\n"
    << "    </div>"
    << "    <p>The goldbach conjecture was a mathematical statement made"
    << "      by Christian Goldbach.</p><br>\n"
    << "    <div class=\"imgbox\">"
    << "      <img class=\"centerFit\" src=\"https://i.imgur.com/21vGOe4.jpg\""
    << "        alt=\"Goldbach\" width=\"400\" height=\"450\">"
    << "    </div>"
    << "    <br><p>This conjecture consists of the following:</p>\n"
    << "    <p>Given a number greater than 5:</p>\n"
    << "    <p>If the number is even, it can be written as the sum of"
    << "      two prime numbers. For example: 6 = 3 + 3 and 14 = 7 + 7."
    << "      This is known as the Strong Conjecture.</p>\n"
    << "    <p>If the number is odd, it can be written as the sum of"
    << "      three prime numbers. For example: 7 = 2 + 2 + 3, and"
    << "      9 = 2 + 2 + 5. This is known as the Weak Conjecture."
    << "      </p>\n"
    << "    <div class=\"centerTitle\">"
    << "      <h1>How does this calculator work?</h1>\n"
    << "    </div>"
    << "    <p>The user needs to write a number, or if more than one,"
    << "      separated by ','. The numbers can be positive or negative,"
    << "      but on the inside all numbers are treated as positive, the"
    << "      minus sign (-) works as a specifier to know if the"
    << "      user wants to see the sums of the entered number. In other"
    << "      words:</p>\n"
    << "    <p>For positive numbers, only the number of sums will"
    << "      be displayed. Example: 21: 5 sums.</p>\n"
    << "    <p>For negative numbers, it will display the number of sums"
    << "      and the actual sums related to the number and its respective"
    << "      conjecture. Example: -21: 5 sums: 2 + 2 + 17, 3 + 5 + 13, "
    << "      3 + 7 + 11, 5 + 5 + 11, 7 + 7 + 7.</p><hr width=\"96%\">\n"
    << "    <div class=\"centerTitle\">"
    << "      <p><a href=\"/\">Back</a></p>\n"  // Link back to homepage
    << "    </div>"
    << "  </div>\n"
    << "</html>\n";

  // Send the response to the client (user agent)
  return httpResponse.send();
}

/**
 * @brief Shows the "Not found" page if the input is invalid.
 * @return httpResponse Contains the data the browser needs to show the page.
 */

bool GoldbachWebApp::serveNotFound(HttpRequest& httpRequest
  , HttpResponse& httpResponse) {
  (void)httpRequest;

  // Set HTTP response metadata (headers)
  httpResponse.setStatusCode(404);

  // Build the body of the response
  std::string title = "Not found";  // Page title
  httpResponse.body() << "<!DOCTYPE html>\n"
    << "<html lang=\"en\">\n"
    << "  <meta charset=\"ascii\"/>\n"
    << "  <meta name=\"viewport\" content=\"width=device-width,"
    << "    initial-scale=1.0\">\n"
    << "  <title>" << title << "</title>\n"
    << "  <style>\n"  // Page configuration
    << "    body { font-family: monospace;\n"
    << "      background-color:black;\n"
    << "      text-align:center; }\n"
    << "    h1 { font-size: 32px;\n"
    << "      color:red; }\n"
    << "    hr { color:white; }\n"
    << "    p { color:white;\n"
    << "      font-size: 20px; }\n"
    << "    a { color:white; }\n"
    << "    .center {\n"  // Used in Div to center page content
    << "      margin: 0;\n"
    << "      position: absolute;\n"
    << "      top: 50%;\n"
    << "      left: 50%;\n"
    << "      transform: translate(-50%, -50%); }\n"
    << "  </style>\n"
    << "  <div class=\"center\">\n"  // Page content
    << "    <h1>" << title << "</h1>\n"  // Title
    << "    <p>The requested resouce was not found in"
    << "      this server.</p>\n"  // Message
    << "    <hr><p><a href=\"/\">Homepage</a></p>\n"  // Link back to Homepage
    << "  </div>\n"
    << "</html>\n";

  // Send the response to the client (user agent)
  return httpResponse.send();
}

/**
 * @brief Shows the "Out of range" page if the number if larger than int64_t.
 * @return httpResponse Contains the data the browser needs to show the page.
 */

bool GoldbachWebApp::serveOutOfRange(HttpRequest& httpRequest
  , HttpResponse& httpResponse) {
  (void)httpRequest;

  // Build the body of the response
  std::string title = "Out of range";  // Page title
  httpResponse.body() << "<!DOCTYPE html>\n"
    << "<html lang=\"en\">\n"
    << "  <meta charset=\"ascii\"/>\n"
    << "  <meta name=\"viewport\" content=\"width=device-width,"
    << "    initial-scale=1.0\">\n"
    << "  <title>" << title << "</title>\n"
    << "  <style>\n"  // Page configuration
    << "    body { font-family: monospace;\n"
    << "      background-color:black;\n"
    << "      text-align:center; }\n"
    << "    h1 { font-size: 32px;\n"
    << "      color:red; }\n"
    << "    hr { color:white; }\n"
    << "    p { color:white;\n"
    << "      font-size: 20px; }\n"
    << "    a { color:white; }\n"
    << "    .center {\n"  // Used in Div to center page content
    << "      margin: 0;\n"
    << "      position: absolute;\n"
    << "      top: 50%;\n"
    << "      left: 50%;\n"
    << "      transform: translate(-50%, -50%); }\n"
    << "  </style>\n"
    << "  <div class=\"center\">\n"  // Page content
    << "    <h1>" << title << "</h1>\n"  // Title
    << "    <p>The requested number was out of range.</p>\n"  // Message
    << "    <p>Accepted range: -9.223.372.036.854.775.807 to "
    << "      9.223.372.036.854.775.807.</p>\n"  // Message
    << "    <hr><p><a href=\"/\">Homepage</a></p>\n"  // Link back to Homepage
    << "  </div>\n"
    << "</html>\n";

  // Send the response to the client (user agent)
  return httpResponse.send();
}

/**
 * @brief Shows the "Goldbach sums for (numbers)" page for valid inputs.
 * @param numbers Vector containing the numbers to calculate the sums for.
 * @return httpResponse Contains the data the browser needs to show the page.
 */
bool GoldbachWebApp::serveGoldbachSums(HttpRequest& httpRequest
    , HttpResponse& httpResponse, std::vector<int64_t> numbers) {
  (void)httpRequest;
  // Build the body of the response
  std::string title = "Goldbach sums for ";  // Page title
  for (size_t i = 0; i < numbers.size(); i++) {
    title = title + std::to_string(numbers[i]);
    if (i + 1 != numbers.size()) {
      title = title + ", ";
    }
  }

  httpResponse.body() << "<!DOCTYPE html>\n"
    << "<html lang=\"en\">\n"
    << "  <meta charset=\"ascii\"/>\n"
    << "  <meta name=\"viewport\" content=\"width=device-width,"
    << "    initial-scale=1.0\">\n"
    << "  <title>" << title << "</title>\n"
    << "  <style>\n"  // Page configuration
    << "    body { font-family: monospace;\n"
    << "      background-color:black;\n"
    << "      text-align:center; }\n"
    << "    h1 { font-size: 32px;\n"
    << "      color:white; }\n"
    << "    h2 { font-size: 24px;\n"
    << "      color:white; }\n"
    << "    hr { color:white; }\n"
    << "    p { color:white;\n"
    << "      font-size: 18px; }\n"
    << "    ol { color:white; }\n"
    << "    li { font-size: 18px; }\n"
    << "    a { color:white; }\n"
    << "    label { color:white; }\n"
    << "    .centerAll {\n"  // Used in Div to center page content
    << "      position: absolute;\n"
    << "      top: 0;\n"
    << "      left: 0;\n"
    << "      right: 0;\n"
    << "      bottom: 0;\n"
    << "      display: flex;\n"
    << "      flex-direction: column; }\n"
    << "    .centerTitle {\n"  // Used in Div to center page title
    << "      margin: auto; }\n"
    << "    .centerResult {\n"  // Used in Div to center each number's sums
    << "      margin: auto;\n"
    << "      width: max-content; }\n"
    << "    .err { color: red; }\n"  // Used to show an invalid number in red
    << "    </style>\n"
    << "    <div class=\"centerAll\">"  // Page content
    << "      <div class=\"centerTitle\">"  // Center Title
    << "        <h1>" << title << "</h1>\n";  // Title
  Semaphore*  canContinue = new Semaphore(0);
  size_t doneNumbersCounter = 0;
  goldbachSums* responseArray = new goldbachSums[numbers.size()];
  for (size_t index = 0; index < numbers.size(); index++) {
    goldbachSums temporal;
    temporal.numberGoldbach = numbers[index];
    temporal.sumsCounter = 0;
    temporal.originVectorPosition = index;
    if (numbers[index] < -5 || numbers[index] > 5) {
      if (numbers[index] < -5) {
        temporal.numberGoldbach = temporal.numberGoldbach * -1;
        temporal.sign = 1;
      } else {
        temporal.sign = 0;
      }
      temporal.isValid = true;
    } else {
      temporal.isValid = false;
    }
    temporal.stopCondition = 1;
    temporal.numbersQuantity = numbers.size();
    temporal.responseArray = responseArray;
    temporal.canContinue =  canContinue;
    temporal.doneNumbers = &doneNumbersCounter;
    goldbachWorkQueue->push(temporal);
  }
  canContinue->wait();
  bool response = fillSums(responseArray, httpResponse, numbers.size());
  delete[] responseArray;
  delete canContinue;
  return response;
}

bool GoldbachWebApp::fillSums(goldbachSums* responseArray,
HttpResponse& httpResponse, size_t numbersLenght) {
  for (size_t index = 0; index < numbersLenght; index++) {
      httpResponse.body() << "<div class=\"centerResult\">\n";
      if (responseArray[index].isValid == true) {
        // If the user doesn't want the list of sums
        if (responseArray[index].sign == 0) {
          httpResponse.body() << "<h2>" << std::to_string(responseArray[index]
          .numberGoldbach) << "</h2>\n" << "<p>" << std::to_string
          (responseArray[index].numberGoldbach) << ": " << std::to_string
          (responseArray[index].sumsCounter) << " sums</p>\n";
          // Shows counter only
        } else {  // If the user wants the list of sums
          httpResponse.body() << "<h2>-" << std::to_string(responseArray[index]
          .numberGoldbach) << "</h2>\n" << "<p>-" << std::to_string
          (responseArray[index].numberGoldbach) << ": " << std::to_string
          (responseArray[index].sumsCounter) << " sums</p>\n" << "<ol>\n";
          // Shows both counter and list
          for (int64_t index2 = 0; index2 < responseArray[index].sumsCounter;
          index2++) {
            // Shows two elements list
            if (responseArray[index].sums[index2].num3 == 0) {
              httpResponse.body() << "<li>" << std::to_string
              (responseArray[index].sums[index2].num1) << " + " <<
              std::to_string(responseArray[index].sums[index2].num2)
              << "</li>\n";
            } else {  // Shows three elements list
              httpResponse.body() << "<li>" << std::to_string
              (responseArray[index].sums[index2].num1) << " + " <<
              std::to_string(responseArray[index].sums[index2].num2)
              << " + " << std::to_string(responseArray[index].sums[index2]
              .num3) << "</li>\n";
            }
          }
          httpResponse.body()  << "</ol>\n";  // Ends reading the list
        }
      } else {  // If number is not valid, shows NA
        httpResponse.body() << "<h2 class=\"err\">" <<
        std::to_string(responseArray[index].numberGoldbach)
        << "</h2>\n" << "<p>" << std::to_string(responseArray[index]
        .numberGoldbach) << ": NA</p>\n";
      }
      httpResponse.body() << "</div>\n";
    }
    // Link back to homepage
    httpResponse.body() << "<hr><p><a href=\"/\">Back</a></p>\n"
      << "    </div>\n"
      << "  </div>\n"
      << "</html>\n";
  return httpResponse.send();
}
