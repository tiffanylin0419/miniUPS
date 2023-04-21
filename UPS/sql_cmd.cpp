#include <iostream>
#include <string>
#include <memory>
#include <pqxx/pqxx>
#include <chrono>
#include <ctime>

using namespace std;

//create truck
void Ucreate_truck_sql(int truck_id, int world_id, int loc_x, int loc_y) {
    // connect to database
    pqxx::connection conn("dbname=postgres user=postgres password=Andy860812! hostaddr=127.0.0.1 port=5432");

    try {
        // execute a query to check if the truck already exists
        pqxx::work txn(conn);
        pqxx::result res = txn.exec("SELECT * FROM ups_truck WHERE truck_id=" + to_string(truck_id));

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
void Ufinish_sql(int truck_id, string truck_status, int new_x, int new_y) {
    // connect to database
    pqxx::connection conn("dbname=postgres user=postgres password=Andy860812! hostaddr=127.0.0.1 port=5432");

    try {
        // execute a query to get the truck with the specified ID
        pqxx::work txn(conn);
        pqxx::result res = txn.exec("SELECT * FROM ups_truck WHERE truck_id=" + to_string(truck_id));

        // check if the truck exists
        if (res.empty()) {
            cerr << "Truck with truck_id=" << truck_id << " does not exist." << endl;
            txn.abort();
            return;
        }

        // update the truck's location
        txn.exec("UPDATE ups_truck SET truck_status='"+ truck_status + "', loc_x=" + to_string(new_x) + ", loc_y=" + to_string(new_y) + " WHERE truck_id=" + to_string(truck_id));
        txn.commit();
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}


//update package UDeliveryMade
void UDeliveryMade_sql(int world_id,int truck_id, int package_id) {
    // connect to database
    pqxx::connection conn("dbname=postgres user=postgres password=Andy860812! hostaddr=127.0.0.1 port=5432");

    // Get the current time
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);

    try {
        // execute a query to get the truck with the specified ID
        pqxx::work txn(conn);
        pqxx::result res = txn.exec("SELECT * FROM ups_package WHERE truck_id=" + to_string(truck_id) +" AND package_id=" + to_string(package_id) + " AND world_id=" + to_string(world_id));

        // check if the truck exists
        if (res.empty()) {
            cerr << "Package with world_id="<< world_id <<" truck_id=" << truck_id << " Truck with package_id=" << package_id << " does not exist." << endl;
            txn.abort();
            return;
        }

        // update the truck's location
        txn.exec("UPDATE ups_package SET package_status= 'Delivered', delivered_time = to_timestamp(" + std::to_string(now_c) + ") WHERE truck_id=" + to_string(truck_id) +" AND package_id=" + to_string(package_id) + " AND world_id=" + to_string(world_id));
        txn.commit();
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}

/*
int main(){
    //Ucreate_truck_sql(3, 1, 5, 6);
    //Ucreate_truck_sql(1, "idle", 5, 6);
    //Ufinish_sql(2,"idle" , 3, 4);
    UDeliveryMade_sql(1,1,1);
    return 0;
}
*/