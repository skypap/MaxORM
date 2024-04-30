#include "MaxDB.h"
//#include "DBUtil.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "driver/sdmmc_defs.h"
#include "sdmmc_cmd.h"
MaxDB* MaxDB::dbMax = nullptr;
SQLite::Database* MaxDB::db = nullptr;

const char* MaxDB::TAG="MaxDB";

void MaxDB::initInstance() {
	new MaxDB;
}
MaxDB::MaxDB(){
	if (dbMax != nullptr) {
		std::cout << "Existe deja";
		//throw std::logic_error("Instance already exists");
	}
	else {
		dbMax = this;
    std::string dbName=CONFIG_DB_NAME;
    #ifdef CONFIG_DB_USE_SDCARD
    initSDCard();
    dbName="/sdcard/"+dbName+".db3";

#endif
		db = new SQLite::Database(dbName.c_str(), SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
	}
}

MaxDB& MaxDB::GetInstance() {
	if (dbMax == nullptr) {
		initInstance();
	}
  return *dbMax;
}


int MaxDB::int_CreateTable(const std::string& Table, DBObject* Obj)
{
	std::string Query = "CREATE TABLE IF NOT EXISTS ";
	Query += Table;
	Query += " (";

	Obj->ExportMembers();

//	for (unsigned int i = 0; i < Obj->TableVector.size(); i++)
//	{
//		Query += Obj->TableVector[i].first.first + " ";
//		Query += Obj->TableVector[i].first.second + ", ";
//	}
    for (auto & i : Obj->TableVector)
    {
        Query += i.first.first + " ";
        Query += i.first.second + ", ";
    }
	Obj->TableVector.clear();

	Query.pop_back();
	Query.pop_back();
	Query += ");";
	try {
		return db->exec(Query.c_str());
	}
	catch (std::exception& e) {
		std::cout << "SQLite exception: " << e.what() << std::endl;
		return db->getErrorCode();
	}
}

int MaxDB::int_DeleteTable(const std::string& Table)
{
	std::string Query = "DROP TABLE " + Table + ";";
	try {
		return db->exec(Query.c_str());
	}
	catch (std::exception& e) {
		std::cout << "SQLite exception: " << e.what() << std::endl;
		return db->getErrorCode();
	}
}

int MaxDB::int_ClearTable(const std::string& Table)
{
	std::string Query = "DELETE FROM " + Table + ";";
	try {
		return db->exec(Query.c_str());
	}
	catch (std::exception& e) {
		std::cout << "SQLite exception: " << e.what() << std::endl;
		return db->getErrorCode();
	}
}

int MaxDB::int_Insert(const std::string& Table, DBObject* Obj)
{
	std::string Query = "INSERT INTO " + Table + " (";

	Obj->ExportMembers();

//	for (unsigned int i = 0; i < Obj->TableVector.size(); i++)
//		Query += Obj->TableVector[i].first.first + ", ";

    for (auto & i : Obj->TableVector){
		Query += i.first.first + ", ";
    }

	Query.pop_back();
	Query.pop_back();

	Query += ") VALUES (";

//	for (unsigned int i = 0; i < Obj->TableVector.size(); i++)
//		Query += Obj->TableVector[i].second + ", ";

    for (auto & i : Obj->TableVector){
        Query += i.second + ", ";
    }

	Obj->TableVector.clear();

	Query.pop_back();
	Query.pop_back();
	Query += ");";

	try {
		return db->exec(Query.c_str());
	}
	catch (std::exception& e) {
		std::cout << "SQLite exception: " << e.what() << std::endl;
		return db->getErrorCode();
	}
}

int MaxDB::int_Update(const std::string& Table, DBObject* Obj)
{
	std::string Query = "UPDATE " + Table + " SET ";

	Obj->ExportMembers();

//	for (unsigned int i = 0; i < Obj->TableVector.size(); i++)
//	{
//		Query += Obj->TableVector[i].first.first + " = ";
//		Query += Obj->TableVector[i].second + ", ";
//	}

    for (auto & i : Obj->TableVector)
    {
        Query += i.first.first + " = ";
        Query += i.second + ", ";
    }



	Query.pop_back();
	Query.pop_back();
	Query += " WHERE ";
	bool constraintExist = false;
	//Checker si constraint
//	for (unsigned int i = 0; i < Obj->TableVector.size(); i++)
//	{
//		if (Obj->TableVector[i].first.second.find(ConstraintMap[ESQLConstraint::UNIQUE]) != std::string::npos||
//			Obj->TableVector[i].first.second.find(ConstraintMap[ESQLConstraint::PRIMARY_KEY]) != std::string::npos) {
//			constraintExist = true;
//			Query += Obj->TableVector[i].first.first + " = ";
//			Query += Obj->TableVector[i].second + ", ";
//		}
//	}

    for (auto & i : Obj->TableVector)
    {
        if (i.first.second.find(ConstraintMap[ESQLConstraint::UNIQUE]) != std::string::npos||
            i.first.second.find(ConstraintMap[ESQLConstraint::PRIMARY_KEY]) != std::string::npos) {
            constraintExist = true;
            Query += i.first.first + " = ";
            Query += i.second + ", ";
        }
    }

	Obj->TableVector.clear();

	if (!constraintExist) {
		std::cout << "No Constraint = Not Updating";
		return  db->getErrorCode();
	}
	Query.pop_back();
	Query.pop_back();

	try {
		return db->exec(Query.c_str());
	}
	catch (std::exception& e) {
		std::cout << "SQLite exception: " << e.what() << std::endl;
		return db->getErrorCode();
	}
}


int MaxDB::int_Update_Where(const std::string& Table, DBObject* Obj, const std::string& SearchCondition)
{
	std::string Query = "UPDATE " + Table + " SET ";

	Obj->ExportMembers();

//	for (unsigned int i = 0; i < Obj->TableVector.size(); i++)
//	{
//		Query += Obj->TableVector[i].first.first + " = ";
//		Query += Obj->TableVector[i].second + ", ";
//	}

    for (auto & i : Obj->TableVector)
    {
        Query += i.first.first + " = ";
        Query += i.second + ", ";
    }

	Obj->TableVector.clear();

	Query.pop_back();
	Query.pop_back();
	Query += " WHERE ";
	Query += SearchCondition;

	try {
		return db->exec(Query.c_str());
	}
	catch (std::exception& e) {
		std::cout << "SQLite exception: " << e.what() << std::endl;
		return db->getErrorCode();
	}
}

int MaxDB::int_Set(const std::string& Table, DBObject* Obj)
{
	int retour;
	int_Insert(Table,Obj);
	retour =int_Update(Table, Obj);
	return retour;
}


int MaxDB::int_Delete(const std::string& Table, DBObject* Obj) {
	std::string Query = "DELETE FROM " + Table + " WHERE ";
	Obj->ExportMembers();
//	for (unsigned int i = 0; i < Obj->TableVector.size(); i++)
//	{
//		Query += Obj->TableVector[i].first.first + " = ";
//		Query += Obj->TableVector[i].second + ", ";
//	}

    for (auto & i : Obj->TableVector)
    {
        Query += i.first.first + " = ";
        Query += i.second + ", ";
    }
	Obj->TableVector.clear();
	Query.pop_back();
	Query.pop_back();

	try {
		return db->exec(Query.c_str());
	}
	catch (std::exception& e) {
		std::cout << "SQLite exception: " << e.what() << std::endl;
		return db->getErrorCode();
	}

}

int MaxDB::int_Delete_Where(const std::string& Table, const std::string& SearchCondition) {
	std::string Query = "DELETE FROM " + Table + " WHERE " + SearchCondition + " ;";
	try {
		return db->exec(Query.c_str());
	}
	catch (std::exception& e) {
		std::cout << "SQLite exception: " << e.what() << std::endl;
		return db->getErrorCode();
	}
}

int MaxDB::int_GetWhere(const std::string& Table, DBObject* Obj, const std::string& SearchCondition)
{
	std::string Query = "SELECT * FROM " + Table + " WHERE " + SearchCondition;
	try {
		SQLite::Statement   query(*db, Query.c_str());
		std::vector< std::string> megaRetour ;
		while (query.executeStep()) {
			for (int indx = 0; indx < query.getColumnCount(); indx++) {
				megaRetour.emplace_back(query.getColumn(indx).getText());
			}
		}
		auto Object = static_cast<DBObject*>(Obj);
		Object->ImportMembers(megaRetour);
		return db->getErrorCode();
	}
	catch (std::exception& e) {
		std::cout << "SQLite exception: " << e.what() << std::endl;
		return db->getErrorCode();
	}
}

int MaxDB::int_GetVectWhere(const std::string& Table, std::vector<DBObject*> Vector, const std::string& SearchCondition) {
	std::string Query = "SELECT * FROM " + Table + " WHERE " + SearchCondition;
	try {
		SQLite::Statement   query(*db, Query.c_str());
		std::vector< std::string> megaRetour;

		int i = 0;
		while (query.executeStep()) {
			for (int indx = 0; indx < query.getColumnCount(); indx++) {
				megaRetour.emplace_back(query.getColumn(indx).getText());
			}
			Vector[i]->ImportMembers(megaRetour);
			megaRetour.clear();
			i++;
		}
		return db->getErrorCode();
	}
	catch (std::exception& e) {
		std::cout << "SQLite exception: " << e.what() << std::endl;
		return db->getErrorCode();
	}
}

bool MaxDB::bool_Exist(const std::string& Table, DBObject* Obj) {
	std::string Query = "SELECT EXISTS(SELECT * FROM " + Table + " WHERE ";
	Obj->ExportMembers();

	bool constraintExist = false;
	//Checker si constraint
//	for (unsigned int i = 0; i < Obj->TableVector.size(); i++)
//	{
//		if (Obj->TableVector[i].first.second.find(ConstraintMap[ESQLConstraint::UNIQUE]) != std::string::npos ||
//			Obj->TableVector[i].first.second.find(ConstraintMap[ESQLConstraint::PRIMARY_KEY]) != std::string::npos) {
//			constraintExist = true;
//			Query += Obj->TableVector[i].first.first + " = ";
//			Query += Obj->TableVector[i].second + ", ";
//		}
//	}

    for (auto & i : Obj->TableVector)
    {
        if (i.first.second.find(ConstraintMap[ESQLConstraint::UNIQUE]) != std::string::npos ||
            i.first.second.find(ConstraintMap[ESQLConstraint::PRIMARY_KEY]) != std::string::npos) {
            constraintExist = true;
            Query += i.first.first + " = ";
            Query += i.second + ", ";
        }
    }
	Obj->TableVector.clear();

	try {
		SQLite::Statement  query(*db, Query.c_str());
		query.executeStep();
		return query.getColumn(0).getInt();
	}
	catch (std::exception& e) {
		std::cout << "SQLite exception: " << e.what() << std::endl;
		return false;
	}
}

int MaxDB::Get_Count(const std::string& Table)
{
	std::string Query = "SELECT COUNT(*) FROM " + Table;
	SQLite::Statement  query(*db, Query.c_str());
	query.executeStep();
	int retour = query.getColumn(0).getInt();
	return retour;
}

int MaxDB::Get_Count_Where(const std::string& Table, const std::string& SearchCondition)
{
	std::string Query = "SELECT COUNT(*) FROM " + Table + " WHERE " + SearchCondition + ";";
	SQLite::Statement  query(*db, Query.c_str());
	query.executeStep();
	int retour = query.getColumn(0).getInt();
	return retour;
}

std::vector<std::string> MaxDB::getTablesName() {
  SQLite::Statement   query(*MaxDB::db, "Select tbl_name from sqlite_master WHERE type = 'table'");
  std::vector< std::string> megaRetour ;
  while (query.executeStep()) {
    megaRetour.emplace_back(query.getColumn(0).getText());
  }
  return megaRetour;
}
std::vector<std::pair<std::string,std::string>> MaxDB::getTablesNameCount(){
  std::vector<std::string> vTablesName = getTablesName();
  std::vector<std::pair<std::string,std::string>> megaRetour;
  std::string tmpQuery;
  for(auto e:vTablesName){
    megaRetour.emplace_back(e,std::to_string(Get_Count(e)));
  }
  return megaRetour;
}

#ifdef CONFIG_DB_USE_SDCARD
void MaxDB::initSDCard(){
  esp_vfs_fat_sdmmc_mount_config_t mount_config = {
      .format_if_mount_failed = true,
      .max_files = 5,
      .allocation_unit_size = 16 * 1024 //* 2
  };
  sdmmc_host_t host = SDSPI_HOST_DEFAULT();
  host.max_freq_khz = SDMMC_FREQ_PROBING;
  spi_bus_config_t bus_cfg = {
      .mosi_io_num = CONFIG_SD_MOSI_PIN,
      .miso_io_num = CONFIG_SD_MISO_PIN,
      .sclk_io_num = CONFIG_SD_CLK_PIN,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .data4_io_num=-1,
      .data5_io_num=-1,
      .data6_io_num=-1,
      .data7_io_num=-1,
      .max_transfer_sz = 16 * 1024,
      .flags=0,
      .intr_flags=0

  };

  esp_err_t ret = spi_bus_initialize((spi_host_device_t) host.slot, &bus_cfg, SDSPI_DEFAULT_DMA);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to initialize bus.");
    return;
  }

  sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
  slot_config.gpio_cs = (gpio_num_t) CONFIG_SD_CS_PIN;
  slot_config.host_id = (spi_host_device_t) host.slot;


  ESP_LOGI(TAG, "Mounting filesystem");
  const char mount_point[]="/sdcard";
  sdmmc_card_t *card;
  ret = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_config, &mount_config, &card);
  if (ret != ESP_OK) {
    if (ret == ESP_FAIL) {
      ESP_LOGE(TAG, "Failed to mount filesystem. "
                    "If you want the card to be formatted, set the CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
    } else {
      ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                    "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
    }
    return;
  }
  ESP_LOGI(TAG, "Filesystem mounted");
  sdmmc_card_print_info(stdout, card);
}
#endif