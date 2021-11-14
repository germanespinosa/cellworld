#include<catch.h>
#include<cell_world.h>


using namespace cell_world;
using namespace std;

struct Basic_message_router : Message_router {

    Routes(
Add_route("new_coordinates", Coordinates, new_coordinates);
        Add_route("new_location", Location, new_location);
        Add_route("new_counter", int, new_counter);
        Add_route("new_string", std::string, new_string);
    )

    void new_string (string m){
        cout << "new_string: " << m << endl;
    }

    void new_counter(int c){
        cout << "new_counter: " << c << endl;

    }

    void new_coordinates(const Coordinates &coord){
        cout << "new_coordinates: " << coord << endl;
        nc_c += 1;
    }

    void new_location(const Location &location){
        cout << "new_location: " << location << endl;
        nl_c += 1;
    }

    void failed_route(const Message &m) override {
        cout << "failed route " << m << endl;
    }

    int nc_c = 0;
    int nl_c = 0;
    int ur_c = 0;
    int wf_c = 0;
    int bm_c = 0;
};

TEST_CASE("basic message_router"){
    Basic_message_router bmr;
    string message = Message("new_coordinates",Coordinates(-10,20).to_json()).to_json();
    bmr.on_incoming_data(message);
    message = Message("new_location",Location(-10,20).to_json()).to_json();
    bmr.on_incoming_data(message);
    message = Message("new_counter","5").to_json();
    bmr.on_incoming_data(message);
    message = Message("new_string","LETS TRY THIS").to_json();
    bmr.on_incoming_data(message);
}