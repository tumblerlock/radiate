#pragma once

#include <sstream>

#include <FS.h>
#include <SPIFFS.h>
#include <Arduino.h>

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

#include <logging_handler.h>

class Radiate
{
public:
  IPAddress ip;
  DNSServer dns;
  WebServer web;
  LoggingHandler logger;

  const char *portal_ssid;

  Radiate(const char *portal_ssid);
  Radiate();

  void connectOrPortalize();
  void obliviate();

private:
  // webserver
  void loop();

  // wifi control
  String ssid;
  String password;
  bool station_enabled;
  bool access_point_enabled;
  bool failed_to_connect;
  bool have_credentials;

  void configureWebserver();
  void activatePortal();
  void deactivatePortal();
  bool connectToWifi();
  bool save();
};
