#include<catch.h>
#include<cell_world.h>


using namespace cell_world;
using namespace std;

struct Basic_message_router : Message_router {

    void new_coordinates(const Coordinates &coord){
        cout << "new_coordinates" << coord << endl;
    }

    void routes() override {
        Add_route(coordinates_message, Coordinates, new_coordinates);
    }
};

TEST_CASE("basic message_router"){
    Basic_message_router bmr;
    string message = Message("new_coordinates",Coordinates(-10,20).to_json()).to_json();
    bmr.on_incoming_data(message);
}