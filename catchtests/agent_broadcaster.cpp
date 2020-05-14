#include<catch.h>
#include<cell_world.h>

using namespace std;
using namespace cell_world;

struct Base_Agent : Agent {
    Base_Agent(uint32_t v): Agent ({"base",v}){

    };
    string body;
    const Cell &start_episode(uint32_t) override{

    };
    void update_state(const State &) override{

    };
    Coordinates get_move() override{

    };
    void end_episode(const State &, const History & ) override{

    };
    void receive_message(const Agent_message& m) override {
        body = m.body;
    }
};

TEST_CASE("send_message") {
    Base_Agent a(1),b(2);
    a.send_message({"",0},"1");
    CHECK (a.body == "1");
    CHECK (b.body == "1");
    b.send_message({"",0},"2");
    CHECK (a.body == "2");
    CHECK (b.body == "2");
    a.send_message({"",1},"3");
    CHECK (a.body == "3");
    CHECK (b.body == "2");
    b.send_message({"",1},"4");
    CHECK (a.body == "4");
    CHECK (b.body == "2");
    a.send_message({"",2},"5");
    CHECK (a.body == "4");
    CHECK (b.body == "5");
    b.send_message({"",2},"6");
    CHECK (a.body == "4");
    CHECK (b.body == "6");
    Cell_group g;
    Model m(g);
    m.add_agent(a);
    a.send_message({"",2},"7");
    CHECK (a.body == "4");
    CHECK (b.body == "6");
    a.send_message({"",1},"8");
    CHECK (a.body == "8");
    CHECK (b.body == "6");
    a.send_message({"",0},"9");
    CHECK (a.body == "9");
    CHECK (b.body == "6");

}