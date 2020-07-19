#include<catch.h>
#include<cell_world.h>
#include<iostream>
/*
using namespace cell_world;
using namespace std;

struct Test_Agent : Agent {
    Test_Agent( unsigned int v, Cell &c): cell(c), Agent ({"base",v}){

    };
    string body;
    Cell cell;
    const Cell &start_episode( unsigned int) override{
        return cell;
    };
    void update_state(const State &) override{
        set_status(Action_ready);
    };
    Coordinates get_move(const State &) override{
        return Move{1,0};
    };
    void end_episode(const State &) override{

    };
    void receive_message(const Agent_message& m) override {
        body = m.body;
    }
    Cell get_cell(){
        return data.cell;
    }
};

TEST_CASE("Model status")
{
    World w("test");
    Cell c0(Circle, {0,1},{0,1},0,false);
    Cell c1(Circle, {1,1},{1,1},0,false);
    Cell c2(Circle, {2,1},{2,1},0,false);
    Cell c3(Circle, {3,1},{3,1},0,false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    Cell_group cg = w.create_cell_group();
    Model m(cg);
    Test_Agent a(1,c0),b(2,c0);
    CHECK_THROWS(m.update());
    CHECK_THROWS(m.start_episode());
    m.add_agent(a);
    m.add_agent(b);
    CHECK_NOTHROW(m.start_episode());
    CHECK(a.get_cell().coordinates == Coordinates{0,1});
    CHECK_THROWS(m.start_episode());
    CHECK_NOTHROW(m.update());
    CHECK(a.get_cell().coordinates == Coordinates{1,1});
    CHECK_NOTHROW(m.update());
    CHECK(a.get_cell().coordinates == Coordinates{2,1});
    CHECK_NOTHROW(m.update());
    CHECK(a.get_cell().coordinates == Coordinates{3,1});
    m.end_episode();
    CHECK_THROWS(m.update());
    CHECK_THROWS(m.end_episode());
    CHECK_NOTHROW(m.start_episode());
}*/