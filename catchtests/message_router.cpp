#include<catch.h>
#include<cell_world.h>


using namespace cell_world;
using namespace std;

struct Basic_message_router : Message_router {

    Routes(
Add_route("new_coordinates", new_coordinates, Coordinates);
        Add_route("new_location", new_location, Location);
        Add_route("new_counter", new_counter, int);
        Add_route("new_string", new_string, string);
        Add_route("new_empty", new_empty);
    )

    void new_empty(){
        cout << "new_empty " <<endl;
    }

    void new_string (string m){
        cout << "new_string: " << m << endl;
    }

    void new_counter(int c){
        cout << "new_counter: " << c << endl;

    }

    void new_coordinates(Coordinates coord){
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
    message = Message("new_empty").to_json();
    bmr.on_incoming_data(message);
}
