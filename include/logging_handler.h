#pragma once

#include <WebServer.h>

class LoggingHandler : public RequestHandler  {
  public:
    LoggingHandler() { }
    bool canHandle(HTTPMethod method, String uri);
  private:
    static const char * httpMethod(HTTPMethod http_method_int);
} ;
