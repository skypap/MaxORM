//file: main.cpp
#include <driver/sdmmc_types.h>
#include <sdmmc_cmd.h>
#include <driver/sdspi_host.h>
#include <esp_vfs_fat.h>
#include <HttpDBSynch.h>
#include "Arduino.h"
#include "DBCochon.h"
#include "DBCochon2.h"
#include "Arduino.h"
#include "DBCochon.h"
#include "LittleFS.h"
#include "esp_log.h"
#include "DBCochon.h"
#include "WiFi.h"
#include "WebServer.h"
//#include "HttpDBSynch.h"
const char* TAG="main";

HttpDBSynch* httpDB;
WebServer* server;
//extern "C" void app_main()
//{
//  initArduino();
//  psramInit();
//
//  delay(1000);
//  if(psramFound()){
//    ESP_LOGD(TAG,"PSRAM Found");
//  } else {
//    ESP_LOGE(TAG,"PSRAM not Found");
//  }
//  delay(1000);
////  esp_vfs_fat_sdmmc_mount_config_t mount_config = {
////      .format_if_mount_failed = true,
////      .max_files = 5,
////      .allocation_unit_size = 16 * 1024 //* 2
////  };
////  sdmmc_host_t host = SDSPI_HOST_DEFAULT();
////  host.max_freq_khz = SDMMC_FREQ_PROBING;
////  spi_bus_config_t bus_cfg = {
////      .mosi_io_num = CONFIG_SD_MOSI_PIN,
////      .miso_io_num = CONFIG_SD_MISO_PIN,
////      .sclk_io_num = CONFIG_SD_CLK_PIN,
////      .quadwp_io_num = -1,
////      .quadhd_io_num = -1,
////      .data4_io_num=-1,
////      .data5_io_num=-1,
////      .data6_io_num=-1,
////      .data7_io_num=-1,
////      .max_transfer_sz = 16 * 1024,
////      .flags=0,
////      .intr_flags=0
////
////  };
////
////  esp_err_t ret = spi_bus_initialize((spi_host_device_t) host.slot, &bus_cfg, SDSPI_DEFAULT_DMA);
////  if (ret != ESP_OK) {
////    ESP_LOGE(TAG, "Failed to initialize bus.");
////    return;
////  }
////
////  sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
////  slot_config.gpio_cs = (gpio_num_t) CONFIG_SD_CS_PIN;
////  slot_config.host_id = (spi_host_device_t) host.slot;
////
////
////  ESP_LOGI(TAG, "Mounting filesystem");
////  const char mount_point[]="/sdcard";
////  sdmmc_card_t *card;
////  ret = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_config, &mount_config, &card);
////  if (ret != ESP_OK) {
////    if (ret == ESP_FAIL) {
////      ESP_LOGE(TAG, "Failed to mount filesystem. "
////                    "If you want the card to be formatted, set the CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
////    } else {
////      ESP_LOGE(TAG, "Failed to initialize the card (%s). "
////                    "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
////    }
////    return;
////  }
////  ESP_LOGI(TAG, "Filesystem mounted");
////  sdmmc_card_print_info(stdout, card);
////
//  DBCochon dbc;
//  delay(1000);
//
//  Cochon c, c1;
//
//  c.CochonID = 2;
//  c.NomCochon = "Jean";
//  c.TagID = 11;
//  c.TagID2 = 0;
//  c.DateModif = 10;
//  dbc.Set(&c);
////  dbp.Set(&p);
//  c.TagID = 3;
//  dbc.Update(&c);
//  std::vector<Cochon*> DBDATA = dbc.GetVectWhere<Cochon>("NomCochon='Jean'");
//  dbc.Exist(&c);
//  std::cout << dbc.Exist(&c) << std::endl;
//
//  std::cout << sizeof(c) << std::endl;
//  std::cout << sizeof(dbc) << std::endl;
//  //c1 = dbc.GetWhereID(1);
//  std::cout << "Hello World!\n";
//  // Arduino-like setup()
////  Serial.begin(115200);
////  while(!Serial){
////    ; // wait for serial port to connect
////  }
//  std::cout << "Hello World!\n";
//
//
//
////  if(!LittleFS.begin()){
////    Serial.println("An Error has occurred while mounting SPIFFS");
////    return;
////  }
////  WiFi.begin("Jy69","jygatech");
////  server =new WebServer(80);
//  delay(1000);
////  server->begin();
////  httpDB = new HttpDBSynch(server);
//
//
////  SQLite::Statement   query(*MaxDB::db, "Select tbl_name from sqlite_master WHERE type = 'table'");
////  std::vector< std::string> megaRetour ;
////  while (query.executeStep()) {
////    megaRetour.push_back(query.getColumn(0).getText());
////  }
////  for(auto i:megaRetour){
////    std::cout <<i.c_str()<<"\n";
////  }
//
//  while(true){
////      server->handleClient();
//      std::cout<<"Memory available in PSRAM : " <<ESP.getFreePsram()<<"\n";
//      std::cout<<"Heap size: "<< ESP.getHeapSize()<<"\n";
//      std::cout<<"Free Heap: "<< esp_get_free_heap_size()<<"\n";
//      std::cout<<"Min Free Heap: "<< esp_get_minimum_free_heap_size()<<"\n";
//      std::cout<<"Max Alloc Heap: "<< ESP.getMaxAllocHeap()<<"\n";
//      delay(1000);
//  }
//}
DBCochon *dbc;
DBCochon2 *dbc2;

Cochon *c, *c1;
void setup(){

  ESP_LOGD(TAG,"ALLO Found");
  dbc = new DBCochon();
  dbc2 = new DBCochon2();
  c=new Cochon();

  c->CochonID = 2;
  c->NomCochon = "Jean";
  c->TagID = 11;
  c->TagID2 = 0;
  c->DateModif = 10;
  dbc->Set(c);
  c->TagID = 3;
  dbc->Update(c);
  std::vector<Cochon*> DBDATA = dbc->GetVectWhere<Cochon>("NomCochon='Jean'");
  dbc->Exist(c);
  std::cout << dbc->Exist(c) << std::endl;

  std::cout << sizeof(c) << std::endl;
  std::cout << sizeof(dbc) << std::endl;
//  std::cout << "Hello World!\n";
  // Arduino-like setup()
//  Serial.begin(115200);
//  while(!Serial){
//    ; // wait for serial port to connect
//  }
//  ps_malloc()
  std::cout << "Hello World!\n";

  if(psramFound()){
    ESP_LOGD(TAG,"PSRAM Found");
  } else {
    ESP_LOGE(TAG,"PSRAM not Found");
  }

//  if(!LittleFS.begin()){
//    Serial.println("An Error has occurred while mounting SPIFFS");
//    return;
//  }
  WiFi.begin("Jy69","jygatech");
  server =new WebServer(80);
  delay(1000);
  server->begin();
  httpDB = new HttpDBSynch(server);

//
  SQLite::Statement   query(*MaxDB::db, "Select tbl_name from sqlite_master WHERE type = 'table'");
  std::vector< std::string> megaRetour ;
  while (query.executeStep()) {
    megaRetour.push_back(query.getColumn(0).getText());
  }
  for(auto i:megaRetour){
    std::cout <<i.c_str()<<"\n";
  }
};

void loop(){
  server->handleClient();
  std::cout<<"Memory available in PSRAM : " <<ESP.getFreePsram()<<"\n";
  std::cout<<"Heap size: "<< ESP.getHeapSize()<<"\n";
  std::cout<<"Free Heap: "<< esp_get_free_heap_size()<<"\n";
  std::cout<<"Min Free Heap: "<< esp_get_minimum_free_heap_size()<<"\n";
  std::cout<<"Max Alloc Heap: "<< ESP.getMaxAllocHeap()<<"\n";
  delay(1000);
};