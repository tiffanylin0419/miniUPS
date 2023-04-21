#ifndef UPS_SQL_H
#define UPS_SQL_H

#include <string>

void Ucreate_truck_sql(int truck_id, int world_id, int loc_x, int loc_y);
//update truck truck_status, loc_x, loc_y
void Ufinish_sql(int truck_id, std::string truck_status, int new_x, int new_y);
void UDeliveryMade_sql(int world_id,int truck_id, int package_id);
void UTruck_sql(int world_id, int truck_id, string truck_status, int loc_x, int loc_y);

#endif //UPS_SQL_H
