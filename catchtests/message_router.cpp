#include<catch.h>
#include<cell_world.h>


using namespace cell_world;
using namespace std;

int ne_c = 0;
int nc_c = 0;
int nl_c = 0;
int ur_c = 0;
int wf_c = 0;
int bm_c = 0;

struct Basic_message_router : Message_router {

    Routes(
Add_route("new_coordinates", new_coordinates, Coordinates);
        Add_route("new_location", new_location, Location);
        Add_route("new_counter", new_counter, int);
        Add_route("new_string", new_string, string);
        Add_route("new_empty", new_empty);
        Add_route("new_request", new_request);
    )



    void new_request (){
        send_message({"new_response"});
    }

    Message new_empty(){
        cout << "new_empty " <<endl;
        ne_c ++ ;
        return {"HOLA","TWO"};
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

TEST_CASE("Client message routing") {
    easy_tcp::Server<Basic_message_router> server;
    CHECK(server.start(8500));
    Message_client client;
    CHECK(client.connect("127.0.0.1", 8500));
    client.send_message({"new_request"});
    {
        while (client.messages.empty());
        auto m = client.messages.front();
        cout << m << endl;
        client.messages.pop();
    }
    client.disconnect();
    server.stop();
}
