//
// Created by Proprio on 2023-07-24.
//

#ifndef ESPSQLITECOMPONENT_JSONQUERY_H
#define ESPSQLITECOMPONENT_JSONQUERY_H
//#include "nlohmann/json.hpp"
#include "zlib.h"
#include "json.hpp"
using json = nlohmann::json;
enum class GenericQueryError{OK,QUERY_TOO_LARGE,UNKNOWN};

class JsonQuery{
private:
  static const char* TAG;
  static uint8_t* megaBufferPSRAM;
  static int megaBufferPtr;
  static uint8_t* megaBufferCompressPSRAM;
  static int megaBufferCompressPtr;
public:
  static std::string selectQuery(const std::string& queryStr);
  static GenericQueryError selectQueryCompress(const std::string& queryStr);

  static void setUpdateQuery(const std::string& queryStr);
  static std::string buildHtmlTable(const std::string& queryStr);

  // static int taille;
  static std::string compress_string(const std::string str, int compressionlevel = 1);
  static void compress_char(uint8_t *buff, int size,int compressionlevel = 1);
  static std::string decompress_string(const std::string str);
  static bool init();
  static uint8_t* getMegaBufferCompressPtr(){
    return megaBufferCompressPSRAM;
  }
  static int getMegaBufferCompressSize(){
    return megaBufferCompressPtr;
  }
};


#endif //ESPSQLITECOMPONENT_JSONQUERY_H
