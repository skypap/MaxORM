//
// Created by Proprio on 2023-07-24.
//

#include "JsonQuery.h"
#include "Arduino.h"
#include "SQLiteCpp.h"
#include <iostream>
#include "MaxDB.h"
#define MEGA_BUFFER_PSRAM_SIZE 200000
#define MEGA_BUFFER_COMPRESS_PSRAM_SIZE 20000
#define WINDOW_SIZE 9
#define MEM_LEVEL 1
#define GZIP_ENCODING 16
#define CHUNK 16384

const char* JsonQuery::TAG = "GenericQuery";


uint8_t* JsonQuery::megaBufferPSRAM=nullptr;
int JsonQuery::megaBufferPtr=0;
uint8_t* JsonQuery::megaBufferCompressPSRAM=nullptr;
int JsonQuery::megaBufferCompressPtr=0;
bool JsonQuery::init(){
  megaBufferPSRAM = (uint8_t*)ps_malloc(MEGA_BUFFER_PSRAM_SIZE);
  megaBufferCompressPSRAM = (uint8_t*)ps_malloc(MEGA_BUFFER_COMPRESS_PSRAM_SIZE);
  return true;
}

std::string JsonQuery::selectQuery(const std::string& queryStr) {
  ESP_LOGI(TAG,"%s",queryStr.c_str());

  try {
    SQLite::Statement query(*MaxDB::db, queryStr);
    json jtmp;
    json jArr;
    while (query.executeStep()){
      for (int indx = 0; indx < query.getColumnCount(); indx++) {
        jtmp[query.getColumnName(indx)] = query.getColumn(indx).getText();
      }
      jArr.push_back(jtmp);
    }
    return jArr.dump().c_str();
  } catch (std::exception& e) {
    std::cout << "SQLite exception: " << e.what() << std::endl;
    return "";
  }
}

GenericQueryError JsonQuery::selectQueryCompress(const std::string& queryStr) {
  ESP_LOGI(TAG,"%s",queryStr.c_str());

  megaBufferPtr=0;
  try {
    SQLite::Statement query(*MaxDB::db, queryStr);
    json jtmp;
    //json jArr;
    // uint8_t b= '[';
    int nbObj=0;
    memcpy(megaBufferPSRAM+megaBufferPtr, "[",1);
    megaBufferPtr++;
    while (query.executeStep()){
      nbObj++;
      // memcpy(megaBufferPSRAM+megaBufferPtr, "[",1);
      // megaBufferPtr++;
      for (int indx = 0; indx < query.getColumnCount(); indx++) {
        jtmp[query.getColumnName(indx)] = query.getColumn(indx).getText();
      }
      memcpy(megaBufferPSRAM+megaBufferPtr,jtmp.dump().c_str(),jtmp.dump().size());
      megaBufferPtr+=jtmp.dump().size();
      // memcpy(megaBufferPSRAM+megaBufferPtr, "],",2);
      memcpy(megaBufferPSRAM+megaBufferPtr, ",",1);
      megaBufferPtr++;
      // megaBufferPtr+=2;
    }
    megaBufferPtr--;
    memcpy(megaBufferPSRAM+megaBufferPtr, "]",1);
    megaBufferPtr++;
    if(nbObj==0){
      memcpy(megaBufferPSRAM,"[{}]",4);
      megaBufferPtr=4;
      compress_char(megaBufferPSRAM,megaBufferPtr);
    } else{
      compress_char(megaBufferPSRAM,megaBufferPtr);
    }
    return GenericQueryError::OK;
  } catch (std::exception& e) {
    std::cout << "SQLite exception: " << e.what() << std::endl;
    return GenericQueryError::UNKNOWN;
  }
}


void JsonQuery::setUpdateQuery(const std::string& queryStr) {
  try {
    ESP_LOGI(TAG,"%s",queryStr.c_str());
//    int nb = MaxDB::db->exec(queryStr.c_str());
    MaxDB::db->exec(queryStr.c_str());
  } catch (std::exception& e) {
    std::cout << "SQLite exception: " << e.what() << std::endl;
  }
}

std::string JsonQuery::buildHtmlTable(const std::string& queryStr){
  try {
    SQLite::Statement query(*MaxDB::db, queryStr);
    bool titleFlag = false;
    std::string retour = std::string("<style type=\"text/css\">")+
                         std::string("table {border:ridge 5px Indigo;}")+
                         std::string("table td {border:inset 1px #000;}")+
                         std::string("table tr  {background-color:DarkGray ; color:DarkSlateBlue;}")+
                         std::string("</style>");
    retour +="<table>";
    while (query.executeStep()){
      if(titleFlag==false){
        retour += "<tr>";
        for (int indx = 0; indx < query.getColumnCount(); indx++) {
          retour +="<th>";
          retour += query.getColumn(indx).getName();
          retour +="</th>";
        }
        retour += "</tr>";
        titleFlag=true;
      }
      retour += "<tr>";
      for (int indx = 0; indx < query.getColumnCount(); indx++) {
        retour +="<th>";
        retour += query.getColumn(indx).getText();
        retour +="</th>";
      }
      retour += "</tr>";
    }
    retour+="</table>";
    return retour+="<a href=\"url\"> https://www.epochconverter.com/</a>";
  } catch (std::exception& e) {
    std::cout << "SQLite exception: " << e.what() << std::endl;
  }
  return "";
}




std::string JsonQuery::compress_string(const std::string str,
                                          int compressionlevel)
{
  z_stream zs;                        // z_stream is zlib's control structure
  memset(&zs, 0, sizeof(zs));

  if (deflateInit2(&zs,compressionlevel,Z_DEFLATED, WINDOW_SIZE ,MEM_LEVEL,Z_FIXED) != Z_OK)
    throw(std::runtime_error("deflateInit failed while compressing."));

  zs.next_in = (Bytef*)str.data();
  zs.avail_in = str.size();           // set the z_stream's input

  int ret;
  char outbuffer[256];
  std::string outstring;

  // retrieve the compressed bytes blockwise
  do {
    zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
    zs.avail_out = sizeof(outbuffer);

    ret = deflate(&zs, Z_FINISH);
    ESP_LOGD("TAG","hey");
    if (outstring.size() < zs.total_out) {
      // append the block to the output string
      outstring.append(outbuffer,
                       zs.total_out - outstring.size());
    }
  } while (ret == Z_OK);

  deflateEnd(&zs);

  if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
    std::ostringstream oss;
    oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
    throw(std::runtime_error(oss.str()));
  }

  return outstring;
}

std::string JsonQuery::decompress_string(const std::string str)
{
  z_stream zs;                        // z_stream is zlib's control structure
  memset(&zs, 0, sizeof(zs));

  if (inflateInit(&zs) != Z_OK)
    throw(std::runtime_error("inflateInit failed while decompressing."));

  zs.next_in = (Bytef*)str.data();
  zs.avail_in = str.size();

  int ret;
  char outbuffer[1000];
  std::string outstring;

  // get the decompressed bytes blockwise using repeated calls to inflate
  do {
    zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
    zs.avail_out = sizeof(outbuffer);

    ret = inflate(&zs, 0);

    if (outstring.size() < zs.total_out) {
      outstring.append(outbuffer,
                       zs.total_out - outstring.size());
    }

  } while (ret == Z_OK);

  inflateEnd(&zs);

  if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
    std::ostringstream oss;
    oss << "Exception during zlib decompression: (" << ret << ") "
        << zs.msg;
    throw(std::runtime_error(oss.str()));
  }

  return outstring;
}

void JsonQuery::compress_char(uint8_t *buff, int size,
                                 int compressionlevel ){
  ulong srcLen = size;      // +1 for the trailing `\0`
  ulong destLen = compressBound(srcLen); // this is how you should estimate size
  // needed for the buffer
  unsigned char* ostream = (unsigned char*) ps_malloc(destLen);
  int res = compress(ostream, &destLen, buff, srcLen);
  // destLen is now the size of actuall buffer needed for compression
  // you don't want to uncompress whole buffer later, just the used part
  if(res == Z_BUF_ERROR){
    printf("Buffer was too small!\n");
    // return 1;
  }
  if(res ==  Z_MEM_ERROR){
    printf("Not enough memory for compression!\n");
    //return 2;
  }

  // const unsigned char *i2stream = ostream;
  // unsigned char* o2stream = (unsigned char*) ps_malloc(srcLen);
  // ulong destLen2 = destLen; //destLen is the actual size of the compressed buffer
  // int des = uncompress(o2stream, &srcLen, i2stream, destLen2);
  memcpy(megaBufferCompressPSRAM,ostream,destLen);
  megaBufferCompressPtr = destLen;
  free(ostream);
  // printf("%s\n", o2stream);

}



/*Ancien code*/
// int GenericQuery::tmpQueryCount=0;
// int GenericQuery::tmpQueryObjLeft=0;
// int GenericQuery::tmpQueryObjCount=0;
// SQLite::Statement *GenericQuery::tmpQuery = nullptr;

// const std::string GenericQuery::partialQuery(const std::string queryStr){
//   json jtmp;
//   json jArr;
//   if(tmpQuery==nullptr){
//     //nouvelle query
//     tmpQuery = new SQLite::Statement{*DBBase::db, queryStr};
//     std::string countQuery = queryStr;
//     int ret = uTemplate::ci_find_substr(countQuery,std::string("SeleCt *"));
//     if(ret>=0){
//       countQuery.erase(ret+7,1);
//       countQuery.insert(ret+7,"COUNT(*)");
//       ESP_LOGE("TAG","%s",countQuery.c_str());
//       SQLite::Statement  queryCount(*DBBase::db, countQuery.c_str());
// 		  queryCount.executeStep();
// 		  tmpQueryCount = queryCount.getColumn(0).getInt();
//       tmpQueryObjLeft = tmpQueryCount;
//     }
//   } else if(tmpQuery->isDone()){

//     tmpQueryCount = 0;
//     tmpQueryObjLeft = 0;
//     tmpQuery->~Statement();
//     //nouvelle query
//     tmpQuery = new SQLite::Statement{*DBBase::db, queryStr};
//   }
//   try {
//       while (tmpQuery->executeStep()){
//         for (int indx = 0; indx < tmpQuery->getColumnCount(); indx++) {
//           jtmp[tmpQuery->getColumnName(indx)] = tmpQuery->getColumn(indx).getText();
//         }
//         jArr.push_back(jtmp);
//         if(jArr.dump().size()>16384){//
//           tmpQueryObjCount = (int)jArr.size();
//           tmpQueryObjLeft -=tmpQueryObjCount;
//           ESP_LOGE("TAG","Query elem %i",tmpQueryCount);
//           ESP_LOGE("TAG","obj restant %i",tmpQueryObjLeft);
//           // tmpQuery->ref


//           return jArr.dump();
//         }
//       }
//     }catch (std::exception& e) {
//     std::cout << "SQLite exception: " << e.what() << std::endl;
//     return "";
//   }
// }