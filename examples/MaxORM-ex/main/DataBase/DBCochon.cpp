//
// Created by maxime on 4/29/24.
//

#include "DBCochon.h"

#include "DBCochon.h"
#include "DBUtil.h"
EXPORT_CLASS(Cochon);

void Cochon::ExportMembers() {
  REGISTER_MEMBER_WITH_CONSTRAINT(CochonID, ESQLDataType::INTEGER,ESQLConstraint::UNIQUE);
  REGISTER_MEMBER(NomCochon, ESQLDataType::TEXT);
  REGISTER_MEMBER(TagID, ESQLDataType::INTEGER);
  REGISTER_MEMBER(TagID2, ESQLDataType::INTEGER);
  REGISTER_MEMBER(DateModif, ESQLDataType::INTEGER);

}

void Cochon::ImportMembers(std::vector< std::string> element)
{
  CochonID  = std::stoll(element.at(0));
  NomCochon = element.at(1);
  TagID     = std::stoll(element.at(2));
  TagID2    = std::stoll(element.at(3));
  DateModif = std::stoll(element.at(4));
}

DBCochon::DBCochon() {
  //mdb = MaxDB::GetInstance();
  CreateTable<Cochon>();
  std::cout << "allo";
}