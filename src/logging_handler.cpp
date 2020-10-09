#include "logging_handler.h"

const char * LoggingHandler::httpMethod(HTTPMethod http_method_int) {
  switch( http_method_int ) {
  case HTTPMethod::HTTP_GET:
    return "get";
  case HTTPMethod::HTTP_POST:
    return "post";
  case HTTPMethod::HTTP_DELETE:
    return "delete";
  case HTTPMethod::HTTP_PUT:
    return "put";
  case HTTPMethod::HTTP_PATCH:
    return "patch";
  case HTTPMethod::HTTP_HEAD:
    return "head";
  case HTTPMethod::HTTP_OPTIONS:
    return "options";

  default:
    return "(unknown)";
  }
}


bool LoggingHandler::canHandle(HTTPMethod method, String uri) {
  Serial.printf(
    "(Log) Request %s %s\n",
    httpMethod(method),
    uri.c_str()
  );

  return false;
}
