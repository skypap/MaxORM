//
// Created by maxime on 4/29/24.
//

#pragma once
#include "DBObject.h"
#include "MaxDB.h"
#include <vector>
class Cochon:public DBObject{
    public:
    int64_t CochonID;
    std::string NomCochon;
    int64_t TagID;
    int64_t TagID2;
    int64_t DateModif;
    void ExportMembers() override;
    void ImportMembers(std::vector< std::string> element) override;
};

//Devrai irriter de MaxDB
class DBCochon: public MaxDB{
    public:
    //static void initInstance() {
    //	new DBCochon;
    //}
    //friend class MaxDB;
//protected:
    DBCochon();
    //void Insert(Cochon c);
    //void Update(Cochon c);
    //Cochon GetWhereID(int id);
    //Cochon GetWhereTagID(int id);
    //MaxDB* mdb;

};


