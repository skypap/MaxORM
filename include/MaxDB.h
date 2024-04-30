#pragma once

#include "DBObject.h"
#include "DBUtil.h"
#include "SQLiteCpp.h"


//#define DBNAME "/littlefs/"+CON+".db3"FIG_DB_NAME
class MaxDB
{
public:
  static const char* TAG;

	static MaxDB& GetInstance();

	static void initInstance();
  static std::vector<std::string> getTablesName();
  static std::vector<std::pair<std::string,std::string>> getTablesNameCount();
	template<class C>
	int CreateTable();

	template<class C>
	int DeleteTable();

	template<class C>
	int ClearTable();

	template<class C>
	int Insert(C* Obj);

	template<class C>
	int Update(C* Obj);

	template<class C>
	int UpdateWhere(C* Obj, const std::string& SearchCondition);

	template<class C>
	int Set(C* Obj);

	template<class C>
	int Delete(C* Obj);

	template<class C>
	int DeleteWhere(const std::string& SearchCondition);

	template<class C>
	bool Exist(C* Obj);

	template<class C>
	int Count();

	template<class C>
	int CountWhere(const std::string& SearchCondition);

	template<class C>
	C* GetWhere(const std::string& SearchCondition);

	template<class C>
	std::vector<C*> GetVectWhere(const std::string& SearchCondition);


	static SQLite::Database* db;

private:

	static MaxDB* dbMax;
	static int int_CreateTable(const std::string& Table, DBObject* Obj);

	static int int_DeleteTable(const std::string& Table);

	static int int_ClearTable(const std::string& Table);

	static int int_Insert(const std::string& Table, DBObject* Obj);

	static int int_Update(const std::string& Table, DBObject* Obj);

	static int int_Update_Where(const std::string& Table, DBObject* Obj, const std::string& SearchCondition);

	static int int_Set(const std::string& Table, DBObject* Obj);

	static int int_Delete(const std::string& Table, DBObject* Obj);

	static int int_Delete_Where(const std::string& Table, const std::string& SearchCondition);

	static int int_GetWhere(const std::string& Table, DBObject* Obj, const std::string& SearchCondition);

	static int int_GetVectWhere(const std::string& Table, std::vector<DBObject*> Vector, const std::string& SearchCondition);

	static bool bool_Exist(const std::string& Table, DBObject* Obj);

	static int Get_Count(const std::string& Table);

	static int Get_Count_Where(const std::string& Table, const std::string& SearchCondition);
#ifdef CONFIG_DB_USE_SDCARD
  static void initSDCard();
#endif
protected:
	MaxDB();
};

template<class C>
int MaxDB::CreateTable()
{
	std::string Table = SQLite_ORM_CPP::getClassName<C>();

	auto Object = dynamic_cast<DBObject*>(new C());

    return int_CreateTable(Table, Object);

}

template<class C>
int MaxDB::DeleteTable()
{
	std::string Table = SQLite_ORM_CPP::getClassName<C>();

	return int_DeleteTable(Table);
}


template<class C>
int MaxDB::ClearTable()
{
	std::string Table = SQLite_ORM_CPP::getClassName<C>();

	return int_ClearTable(Table);

	//return ESQLResult::OK;
}

template <class C>
int MaxDB::Insert(C* Obj)
{
	std::string Table = SQLite_ORM_CPP::getClassName<C>();

	return int_Insert(Table, Obj);

	//return ESQLResult::OK;
}

template<class C>
int MaxDB::Update(C* Obj)
{
	std::string Table = SQLite_ORM_CPP::getClassName<C>();

	return int_Update(Table, Obj);

	//return ESQLResult::OK;
}

template<class C>
int MaxDB::UpdateWhere(C* Obj, const std::string& SearchCondition)
{
	std::string Table = SQLite_ORM_CPP::getClassName<C>();

	return int_Update_Where(Table, Obj, SearchCondition);

	//return ESQLResult::OK;
}

template<class C>
int MaxDB::Set(C* Obj)
{
	std::string Table = SQLite_ORM_CPP::getClassName<C>();

	return int_Set(Table, Obj);

	//return ESQLResult::OK;
}

template<class C>
inline int MaxDB::Delete(C* Obj)
{
	std::string Table = SQLite_ORM_CPP::getClassName<C>();

	return int_Delete(Table, Obj);

	//return ESQLResult::OK;
}

template<class C>
inline int MaxDB::DeleteWhere(const std::string& SearchCondition)
{
	std::string Table = SQLite_ORM_CPP::getClassName<C>();

	return int_Delete_Where(Table, SearchCondition);

	//return ESQLResult::OK;
}
//TODO pe passer par reference le vecteur?
template<class C>
C* MaxDB::GetWhere(const std::string& SearchCondition)
{
	std::string Table = SQLite_ORM_CPP::getClassName<C>();

	C* Obj = new C();

	auto ObjPtr = dynamic_cast<DBObject*>(Obj);

//	int result = int_GetWhere(Table, ObjPtr, SearchCondition);
    int_GetWhere(Table, ObjPtr, SearchCondition);
	return Obj;
}
//TODO pe passer par reference le vecteur?

template<class C>
std::vector<C*> MaxDB::GetVectWhere(const std::string& SearchCondition)
{
	std::string Table = SQLite_ORM_CPP::getClassName<C>();

	int ObjectCount = Get_Count_Where(Table, SearchCondition);

	std::vector<C*> Vector;

	std::vector<DBObject*> ModelVector;

	for (int i = 0; i < ObjectCount; i++)
	{
		C* Ptr = new C();

		auto ModelPtr = dynamic_cast<DBObject*>(Ptr);

		Vector.push_back(Ptr);

		ModelVector.push_back(ModelPtr);
	}

//	int result = int_GetVectWhere(Table, ModelVector, SearchCondition);
	int_GetVectWhere(Table, ModelVector, SearchCondition);

	return Vector;
}

template<class C>
bool MaxDB::Exist(C* Obj) {
	std::string Table = SQLite_ORM_CPP::getClassName<C>();
	return bool_Exist(Table,Obj);
}

template<class C>
int MaxDB::Count() {
	std::string Table = SQLite_ORM_CPP::getClassName<C>();
	int ObjectCount = Get_Count(Table);
	return ObjectCount;
}

template<class C>
int MaxDB::CountWhere(const std::string& SearchCondition) {
	std::string Table = SQLite_ORM_CPP::getClassName<C>();
	int ObjectCount = Get_Count_Where(Table, SearchCondition);
	return ObjectCount;
}

