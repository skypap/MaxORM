//
// Created by Proprio on 2023-07-24.
//

#include "HttpDBSynch.h"
#include "JsonQuery.h"
#include "esp_task_wdt.h"

const char* HttpDBSynch::TAG{"HttpDBSynch"};

HttpDBSynch::HttpDBSynch(WebServer *webServer) {
  this->server=webServer;
  JsonQuery::init();
  server->on("/GenericQuery",HTTP_POST,std::bind(&HttpDBSynch::handleGenericQuery, this));
  server->on("/DataBase",HTTP_GET,std::bind(&HttpDBSynch::handleWebDB, this));
  server->on("/DataBaseResponse",HTTP_GET,std::bind(&HttpDBSynch::handleWebDBResponse, this));
}

void HttpDBSynch::handleGenericQuery(){
  if (server->hasArg("SelectQuery")){
    server->send_P(200, TEXT_PLAIN, JsonQuery::selectQuery(server->arg("SelectQuery").c_str()).c_str());
  } else if(server->hasArg("SelectQueryCompress")){
    JsonQuery::selectQueryCompress(server->arg("SelectQueryCompress").c_str());
    //  void WebServer::send_P(int code, PGM_P content_type, PGM_P content, size_t contentLength) {
    server->send_P(200, TEXT_PLAIN,(char*) JsonQuery::getMegaBufferCompressPtr(),JsonQuery::getMegaBufferCompressSize());//Attention char array marche pas sur fichier compresser a cause des drole de symb
  } else if(server->hasArg("SetQuery")){
    JsonQuery::setUpdateQuery(server->arg("SetQuery").c_str());
    server->send(200, TEXT_PLAIN);
  } else if(server->hasArg("testLongRequest")) {
    server->send(404, TEXT_PLAIN, "Not found");
    esp_task_wdt_reset();
  } else{
    server->send(404, TEXT_PLAIN, "Not found");
  }
}

void HttpDBSynch::handleWebDB() {
  std::string tmp = std::string("<!DOCTYPE HTML><html><head>")+
                    std::string("<style type=\"text/css\">")+
                    std::string("table {border:ridge 5px Indigo;}")+
                    std::string("table td {border:inset 1px #000;}")+
                    std::string("table tr  {background-color:DarkGray ; color:DarkSlateBlue;}")+
                    std::string("</style>")+
                    std::string(R"(<meta name="viewport" content="width=device-width, initial-scale=1">)")+
                    std::string("</head><body>")+
                    std::string("<table>")+
                    getTableRow("Table","Count");

  std::vector<std::pair<std::string,std::string>> dbNameCount= MaxDB::getTablesNameCount();
  for (const auto& e: dbNameCount) {
    tmp += getTableRow(e.first, e.second);
  }

  tmp+=std::string("</table>")+
  std::string("<br><br><br><form action=\"/DataBaseResponse\">")+
  std::string(R"(SQL Query: <input type="text" name="inputQuery">)")+
  std::string(R"(<input type="submit" value="Submit">)")+
  std::string("</form><br>")+
  std::string("</body></html>");
  server->send(200, "text/html", tmp.c_str());
}


std::string HttpDBSynch::getTableRow(const std::string& name, const std::string& value){
  std::string tmp = std::string("<tr>")+
                    std::string("<th>")+
                    name +
                    std::string("<th>")+
                    std::string("<th>")+
                    value +
                    std::string("<th>")+
                    std::string("</tr>");
  return tmp;
}

void HttpDBSynch::handleWebDBResponse() {
  if (server->hasArg("inputQuery")){
    std::string tmp = JsonQuery::buildHtmlTable(server->arg("inputQuery").c_str());
    server->send(200, "text/html",  tmp.c_str());
  }
}
