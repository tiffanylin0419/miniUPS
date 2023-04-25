#ifndef UPS_SQL_H
#define UPS_SQL_H

#include <string>
#include <iostream>
#include <memory>
#include <pqxx/pqxx>
#include <chrono>
#include <ctime>

using namespace std;
using namespace pqxx;

void Ucreate_truck_sql(int truck_id, int world_id, int loc_x, int loc_y);
result Ufinish_sql(int world_id, int truck_id, std::string truck_status, int new_x, int new_y);
void UDeliveryMade_sql(int world_id,int truck_id, int package_id);
void UTruck_sql(int world_id, int truck_id, std::string truck_status, int loc_x, int loc_y);
int AUInitPickUp_sql(int world_id, int wh_id, string accountname, int package_id, int addr_x, int addr_y, string description);
result AULoaded_sql(int world_id ,int shipid);

#endif //UPS_SQL_H
