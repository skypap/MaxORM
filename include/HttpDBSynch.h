//
// Created by Proprio on 2023-07-24.
//
#pragma once
//#ifndef ESPSQLITECOMPONENT_HTTPDBSYNCH_H
//#define ESPSQLITECOMPONENT_HTTPDBSYNCH_H
#include "Arduino.h"
#include "WebServer.h"
#include "MaxDB.h"
class HttpDBSynch{
  public:
    HttpDBSynch(WebServer* webServer);

private:
  static const char* TAG;

  void handleGenericQuery();
  void handleWebDB();
  void handleWebDBResponse();
  static std::string getTableRow(const std::string& name, const std::string& value);
//  void handleWebDB();
  WebServer *server;
  const char* TEXT_PLAIN = "text/plain";

};


//#endif //ESPSQLITECOMPONENT_HTTPDBSYNCH_H
