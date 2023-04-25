

#include "sql_cmd.h"

result selectSQL(work &W, string sql){
  result R( W.exec( sql ));
  return R;
}

//create truck
void Ucreate_truck_sql(int truck_id, int world_id, int loc_x, int loc_y) {
    // connect to database
    connection conn("dbname=ups user=postgres password=Andy860812! hostaddr=127.0.0.1 port=5432");

    try {
        // execute a query to check if the truck already exists
        work txn(conn);
        result res = txn.exec("SELECT * FROM ups_truck WHERE truck_id=" + to_string(truck_id));

        // check if the truck already exists
        if (!res.empty()) {
            cerr << "Truck with truck_id=" << truck_id << " already exists." << endl;
            txn.abort();
            return;
        }

        // insert a new truck
        string query = "INSERT INTO ups_truck (truck_id, truck_status, loc_x, loc_y, world_id ) VALUES (" + to_string(truck_id) + ", '" + "idle" + "', " + to_string(loc_x) + ", " + to_string(loc_y) + ", " + to_string(world_id) + ")";
        txn.exec(query);
        txn.commit();
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}



//update truck truck_status, loc_x, loc_y
result Ufinish_sql(int world_id, int truck_id, string truck_status, int new_x, int new_y) {
    // connect to database
    connection conn("dbname=ups user=postgres password=Andy860812! hostaddr=127.0.0.1 port=5432");

    try {
        // get the truck with the specified ID
        work txn(conn);
        result res = txn.exec("SELECT * FROM ups_truck WHERE world_id="+ to_string(world_id) +"AND truck_id=" + to_string(truck_id));
        // check if the truck exists
        if (res.empty()) {
            cerr << "Truck with world_id=" << world_id << " truck_id=" << truck_id << " does not exist." << endl;
            txn.abort();
            return result();
        }
        //For Truck ready 10
        if (truck_status == "loading"){
            txn.exec("UPDATE ups_truck SET truck_status='"+ truck_status + "' WHERE world_id="+ to_string(world_id) + "AND truck_id=" + to_string(truck_id));
            txn.commit();
            txn.exec("UPDATE ups_package SET package_status='loading', load_time=NOW() WHERE world_id=" + to_string(world_id) + " AND truck_id=" + to_string(truck_id));
            txn.commit();
            //return
            work W(conn);
            string sql1="SELECT ... FROM ups_package WHERE world_id=" + to_string(world_id) + " AND truck_id=" + to_string(truck_id) + "AND package_status='loading'";
            result R=selectSQL(W, sql1);
            W.commit();
            return R;
        }
        //for Truck completed 17
        else if(truck_status == "idle"){
            //todo: update truck x & y
            txn.exec("UPDATE ups_truck SET truck_status='"+ truck_status + "', loc_x=NULL, loc_y=NULL, wh_id=NULL WHERE world_id="+ to_string(world_id) + " AND truck_id=" + to_string(truck_id));
            txn.commit();
        }
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
    return result();
}


//update package UDeliveryMade
void UDeliveryMade_sql(int world_id, int truck_id, int package_id) {
    // connect to database
    connection conn("dbname=ups user=postgres password=Andy860812! hostaddr=127.0.0.1 port=5432");

    // Get the current time
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);

    try {
        // execute a query to get the package with the specified ID
        work txn(conn);
        result res = txn.exec("SELECT addr_x, addr_y FROM ups_package WHERE world_id=" + to_string(world_id) + " AND truck_id=" + to_string(truck_id) + " AND package_id=" + to_string(package_id) + " LIMIT 1");

        // check if the package exists
        if (res.empty()) {
            cerr << "Package with world_id=" << world_id << ", truck_id=" << truck_id << ", package_id=" << package_id << " does not exist." << endl;
            txn.abort();
            return;
        }

        // get the package's location
        int loc_x = res[0]["addr_x"].as<int>();
        int loc_y = res[0]["addr_y"].as<int>();

        // update the package's status to Delivered
        txn.exec("UPDATE ups_package SET package_status = 'Delivered', delivered_time = to_timestamp(" + to_string(now_c) + ") WHERE world_id = " + to_string(world_id) + " AND truck_id = " + to_string(truck_id) + " AND package_id = " + to_string(package_id));
        txn.commit();

        // update the truck's location
        txn.exec("UPDATE ups_truck SET loc_x = " + to_string(loc_x) + ", loc_y = " + to_string(loc_y) + " WHERE world_id = " + to_string(world_id) + " AND truck_id = " + to_string(truck_id));
        txn.commit();
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}


//create truck
void UTruck_sql(int world_id, int truck_id, string truck_status, int loc_x, int loc_y) {
    // connect to database
    connection conn("dbname=ups user=postgres password=Andy860812! hostaddr=127.0.0.1 port=5432");

    try {
        // execute a query to check if the truck already exists
        work txn(conn);
        result res = txn.exec("SELECT * FROM ups_truck WHERE world_id=" + to_string(world_id) + " AND truck_id=" + to_string(truck_id));

        // check if the truck already exists
        if (res.empty()) {
            cerr << "Truck with world_id=" << world_id << " truck_id=" << truck_id << " does not exist." << endl;
            txn.abort();
            return;
        }

        // update an existing truck
        txn.exec("UPDATE ups_truck SET truck_status = '" + truck_status + "', loc_x =" + to_string(loc_x) + ", loc_y=" + to_string(loc_y) +  " WHERE world_id=" + to_string(world_id) + " AND truck_id=" + to_string(truck_id));
        txn.commit();
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}
/*

//assignmet 多加 withhouse 
int AUInitPickUp_sql(int world_id, int wh_id, string accountname, int package_id){
    // connect to database
    connection conn("dbname=ups user=postgres password=Andy860812! hostaddr=127.0.0.1 port=5432");
    try {
        work txn(conn);
        //search the truck in traveling and go to the same warehouse 
        result res = txn.exec("SELECT * FROM ups_truck WHERE world_id=" + to_string(world_id) + " AND truck_status='traveling' AND wh_id=" + to_string(wh_id));

        if (!res.empty()) {
            // extract truck_id from the first row
            int truck_id = res.at(0)["truck_id"].as<int>();

            // update packages with the new truck_id
            txn.exec("INSERT INTO ups_package (truck_id, world_id, wh_id, package_id, package_status, amazon_user_name, ready_for_picktime) VALUES (" + to_string(truck_id) + ", " + to_string(world_id) + ", " + to_string(wh_id) + ", " + to_string(package_id) + ", " + "'Pickup'" + ", '" + accountname + "', NOW());");

            return truck_id;
        }
        else{
            result idle_trucks = txn.exec("SELECT * FROM ups_truck WHERE world_id=" + to_string(world_id) + " AND truck_status='idle'");
            if (!idle_trucks.empty()) {
                int truck_id = idle_trucks.at(0)["truck_id"].as<int>();
                txn.exec("UPDATE ups_truck SET truck_status='traveling', wh_id=" + to_string(wh_id) + " WHERE world_id=" + to_string(world_id) + " AND truck_id=" + to_string(truck_id));
                txn.exec("INSERT INTO ups_package (truck_id, world_id, wh_id, package_id, package_status, amazon_user_name, ready_for_picktime) VALUES (" + to_string(truck_id) + ", " + to_string(world_id) + ", " + to_string(wh_id) + ", " + to_string(package_id) + ", " + "'Pickup'" + ", '" + accountname + "', NOW())");
                return truck_id;
            }
            else {
                // handle case where there are no idle trucks
                return -1;
            }
        }
    }
}


void AULoaded_sql(int shipid){
    int package_id = shipid;
    // connect to database
    connection conn("dbname=ups user=postgres password=Andy860812! hostaddr=127.0.0.1 port=5432");
    try {
    work txn(conn);
    //search the truck in traveling and go to the same warehouse 
    result res = txn.exec("SELECT * FROM ups_package WHERE world_id=" + to_string(world_id) + " AND package_id =" + to_string(package_id));
    // check if the truck already exists
    if (res.empty()) {
        cerr << "Package with world_id=" << world_id << " package_id=" << package_id << " does not exist." << endl;
        txn.abort();
        return;
        }
    else{
        txn.exec("UPDATE ups_package SET package_status='delivering' WHERE world_id=" + to_string(world_id) + " AND package_id=" + to_string(package_id));
        int truck_id = res.at(0)["truck_id"].as<int>();
        if (truck_id == NULL){
            cerr << "Package with package_id=" << package_id << "track_id=" << track_id << "does not exist." << endl;
            txn.abort();
            return;
        }else{
            txn.exec("UPDATE ups_truck SET truck_status='delivering' WHERE world_id=" + to_string(world_id) + " AND package_id=" + to_string(package_id));
        }
    }
    
    }

}*/

/*
int main(){
    //Ucreate_truck_sql(3, 1, 5, 6);
    //Ucreate_truck_sql(1, "idle", 5, 6);
    Ufinish_sql(1,1,"idle" , 3, 4);
    //UTruck_sql(1, 6, "traveling" ,2, 2);
    return 0;
}*/