#pragma once

#include <sstream>

#include <logging_handler.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

#include <FS.h>
#include <SPIFFS.h>

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

Radiate::Radiate() : Radiate("Radiate WiFi") {}

Radiate::Radiate(const char *portal_ssid) : portal_ssid(portal_ssid)
{
  failed_to_connect = false;
  have_credentials = false;
  station_enabled = false;
  access_point_enabled = false;

  // allows esp to bring up wifi even before sketch boots
  // set false for development of wifi portal
  WiFi.setAutoConnect(true);
  WiFi.persistent(true);

  IPAddress ip(192, 168, 1, 1);
  DNSServer dns();
  WebServer web(80);
  LoggingHandler logger();

  configureWebserver();
}

void Radiate::configureWebserver()
{
  web.addHandler(&logger);

  web.serveStatic("/portal.html", SPIFFS, "/radiate/portal.html");

  web.on("/favicon.ico", [this]() {
    web.send(404, "text/plain", "icon");
  });

  web.on("/scan.json", [this]() {
    std::stringstream response;

    int network_count = WiFi.scanNetworks();
    response << "{\"networks\": [";

    for (int i = 0; i < network_count; ++i)
    {
      if (i > 0)
        response << ",";
      response << "{";
      response << "\"ssid\": \"" << WiFi.SSID(i).c_str() << "\"";
      response << ',';
      response << "\"rssi\": " << WiFi.RSSI(i);
      response << ',';
      response << "\"encryption_type\": \"" << WiFi.encryptionType(i) << "\"";
      response << "}";
    }

    response << "]}";

    Serial.println(response.str().c_str());

    web.send(200, "application/json", response.str().c_str());
  });

  web.on("/save", HTTPMethod::HTTP_POST, [this]() {
    if (save())
    {
      web.sendHeader("Location", "/done.html", true);
      web.send(301, "text/plain", "Redirecting");
    }
  });

  web.onNotFound([this]() {
    Serial.printf(
        "404 %s -> (redirect to portal)\n",
        web.uri().c_str());

    web.sendHeader("Location", "/portal.html", true);
    web.send(301, "text/plain", "Redirecting");
  });
}

void Radiate::connectOrPortalize()
{
  if (connectToWifi())
    return;

  activatePortal();

  while (!have_credentials || !connectToWifi())
  {
    dns.processNextRequest();
    web.handleClient();
    delay(10);

    if (have_credentials)
    {
      if (connectToWifi())
      {
        break;
      }
      else
      {
        have_credentials = false;
      }
    }
  }

  deactivatePortal();
}

bool Radiate::connectToWifi()
{

  WiFi.enableSTA(true);

  if (have_credentials)
  {
    Serial.printf("Connecting to wifi with provided credentials: %s", ssid.c_str());
    WiFi.begin(ssid.c_str(), password.c_str());
  }
  else
  {
    Serial.printf("Connecting to wifi with pre-stored credentials:");
    WiFi.begin();
  }

  int start_time = millis();
  int status = WiFi.status();
  while (status == WL_DISCONNECTED || status == WL_IDLE_STATUS)
  {
    delay(100);
    status = WiFi.status();
    Serial.print(".");

    if (millis() - start_time > 5000)
    {
      Serial.println("wifi connection timeout..");
      break;
    }
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Connected.");
    return true;
  }
  else
  {
    WiFi.enableSTA(false);
    return false;
  }
}

void Radiate::activatePortal()
{
  Serial.printf("Activating Portal (%s)\n", portal_ssid);

  WiFi.enableAP(true);
  // WiFi.softAPConfig(ip, ip, IPAddress(255, 255, 255, 0));
  WiFi.softAP(portal_ssid);

  while (WiFi.softAPIP().toString() == "0.0.0.0")
  {
    delay(3);
  }

  dns.start(53, "*", WiFi.softAPIP());
  web.begin();
}

void Radiate::deactivatePortal()
{
  Serial.println("Deactivating Portal");
  web.stop();
  dns.stop();
  WiFi.enableAP(false);
}

bool Radiate::save()
{
  ssid = web.arg("ssid");
  password = web.arg("pass");

  if (web.hasArg("chosen_ssid") && web.arg("chosen_ssid") != "Other")
    ssid = web.arg("chosen_ssid");

  have_credentials = true;

  return true;
}

void Radiate::obliviate()
{
  Serial.println("Wiping WiFi credentials...");
  WiFi.persistent(true);
  WiFi.disconnect(true, true);
}

#include <sstream>

#include <logging_handler.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

#include <FS.h>
#include <SPIFFS.h>

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

Radiate::Radiate() : Radiate("Radiate WiFi") {}

Radiate::Radiate(const char *portal_ssid) : portal_ssid(portal_ssid)
{
  failed_to_connect = false;
  have_credentials = false;
  station_enabled = false;
  access_point_enabled = false;

  // allows esp to bring up wifi even before sketch boots
  // set false for development of wifi portal
  WiFi.setAutoConnect(true);
  WiFi.persistent(true);

  IPAddress ip(192, 168, 1, 1);
  DNSServer dns();
  WebServer web(80);
  LoggingHandler logger();

  configureWebserver();
}

void Radiate::configureWebserver()
{
  web.addHandler(&logger);

  web.serveStatic("/portal.html", SPIFFS, "/radiate/portal.html");

  web.on("/favicon.ico", [this]() {
    web.send(404, "text/plain", "icon");
  });

  web.on("/scan.json", [this]() {
    std::stringstream response;

    int network_count = WiFi.scanNetworks();
    response << "{\"networks\": [";

    for (int i = 0; i < network_count; ++i)
    {
      if (i > 0)
        response << ",";
      response << "{";
      response << "\"ssid\": \"" << WiFi.SSID(i).c_str() << "\"";
      response << ',';
      response << "\"rssi\": " << WiFi.RSSI(i);
      response << ',';
      response << "\"encryption_type\": \"" << WiFi.encryptionType(i) << "\"";
      response << "}";
    }

    response << "]}";

    Serial.println(response.str().c_str());

    web.send(200, "application/json", response.str().c_str());
  });

  web.on("/save", HTTPMethod::HTTP_POST, [this]() {
    if (save())
    {
      web.sendHeader("Location", "/done.html", true);
      web.send(301, "text/plain", "Redirecting");
    }
  });

  web.onNotFound([this]() {
    Serial.printf(
        "404 %s -> (redirect to portal)\n",
        web.uri().c_str());

    web.sendHeader("Location", "/portal.html", true);
    web.send(301, "text/plain", "Redirecting");
  });
}

void Radiate::connectOrPortalize()
{
  if (connectToWifi())
    return;

  activatePortal();

  while (!have_credentials || !connectToWifi())
  {
    dns.processNextRequest();
    web.handleClient();
    delay(10);

    if (have_credentials)
    {
      if (connectToWifi())
      {
        break;
      }
      else
      {
        have_credentials = false;
      }
    }
  }

  deactivatePortal();
}

bool Radiate::connectToWifi()
{

  WiFi.enableSTA(true);

  if (have_credentials)
  {
    Serial.printf("Connecting to wifi with provided credentials: %s", ssid.c_str());
    WiFi.begin(ssid.c_str(), password.c_str());
  }
  else
  {
    Serial.printf("Connecting to wifi with pre-stored credentials:");
    WiFi.begin();
  }

  int start_time = millis();
  int status = WiFi.status();
  while (status == WL_DISCONNECTED || status == WL_IDLE_STATUS)
  {
    delay(100);
    status = WiFi.status();
    Serial.print(".");

    if (millis() - start_time > 5000)
    {
      Serial.println("wifi connection timeout..");
      break;
    }
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Connected.");
    return true;
  }
  else
  {
    WiFi.enableSTA(false);
    return false;
  }
}

void Radiate::activatePortal()
{
  Serial.printf("Activating Portal (%s)\n", portal_ssid);

  WiFi.enableAP(true);
  // WiFi.softAPConfig(ip, ip, IPAddress(255, 255, 255, 0));
  WiFi.softAP(portal_ssid);

  while (WiFi.softAPIP().toString() == "0.0.0.0")
  {
    delay(3);
  }

  dns.start(53, "*", WiFi.softAPIP());
  web.begin();
}

void Radiate::deactivatePortal()
{
  Serial.println("Deactivating Portal");
  web.stop();
  dns.stop();
  WiFi.enableAP(false);
}

bool Radiate::save()
{
  ssid = web.arg("ssid");
  password = web.arg("pass");

  if (web.hasArg("chosen_ssid") && web.arg("chosen_ssid") != "Other")
    ssid = web.arg("chosen_ssid");

  have_credentials = true;

  return true;
}

void Radiate::obliviate()
{
  Serial.println("Wiping WiFi credentials...");
  WiFi.persistent(true);
  WiFi.disconnect(true, true);
}
