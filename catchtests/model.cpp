#include<catch.h>
#include<cell_world.h>
#include<iostream>

using namespace cell_world;
using namespace std;

struct Test_Agent : Agent {
    Test_Agent( unsigned int v, Cell &c): cell(c){

    };
    Cell cell;
    Agent_state state;
    const Cell &start_episode() override{
        return cell;
    };
    Agent_status_code update_state(const Model_state &model_state) override{
        state = model_state.agents_state[model_state.current_turn];
        return Agent_status_code::Running;
    };
    Move get_move(const Model_state &) override{
        return {1,0};
    };
    void end_episode(const Model_state &) override{
    };
    virtual Agent_state &get_state_reference(){
        return state;
    };

};
TEST_CASE("Model status")
{
    World w("test");
    Cell c0(Circle, {0,1},{0,1},false);
    Cell c1(Circle, {1,1},{1,1},false);
    Cell c2(Circle, {2,1},{2,1},false);
    Cell c3(Circle, {3,1},{3,1},false);
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
    CHECK(a.get_state_reference().cell.coordinates == Coordinates{0,1});
    CHECK(b.get_state_reference().cell.coordinates == Coordinates{0,1});
    CHECK_THROWS(m.start_episode());
    CHECK_NOTHROW(m.update());
    CHECK(a.get_state_reference().cell.coordinates == Coordinates{1,1});
    CHECK(b.get_state_reference().cell.coordinates == Coordinates{0,1});
    CHECK_NOTHROW(m.update());
    CHECK(a.get_state_reference().cell.coordinates == Coordinates{1,1});
    CHECK(b.get_state_reference().cell.coordinates == Coordinates{1,1});
    CHECK_NOTHROW(m.update());
    CHECK(a.get_state_reference().cell.coordinates == Coordinates{2,1});
    CHECK(b.get_state_reference().cell.coordinates == Coordinates{1,1});
    CHECK_NOTHROW(m.update());
    CHECK(a.get_state_reference().cell.coordinates == Coordinates{2,1});
    CHECK(b.get_state_reference().cell.coordinates == Coordinates{2,1});
    CHECK_NOTHROW(m.update());
    CHECK(a.get_state_reference().cell.coordinates == Coordinates{3,1});
    CHECK(b.get_state_reference().cell.coordinates == Coordinates{2,1});
    CHECK_NOTHROW(m.update());
    CHECK(a.get_state_reference().cell.coordinates == Coordinates{3,1});
    CHECK(b.get_state_reference().cell.coordinates == Coordinates{3,1});
    CHECK_NOTHROW(m.end_episode());
    CHECK_THROWS(m.update());
    CHECK_THROWS(m.end_episode());
}

/*
TEST_CASE("Model reset")
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
    CHECK(a.get_state_reference().cell.coordinates == Coordinates{0,1});
    CHECK(b.get_state_reference().cell.coordinates == Coordinates{0,1});
    CHECK_THROWS(m.start_episode());
    CHECK_NOTHROW(m.update());
    CHECK(a.get_state_reference().cell.coordinates == Coordinates{1,1});
    CHECK(b.get_state_reference().cell.coordinates == Coordinates{0,1});
    Model_state s = m.state;
    cout << s << endl;
    CHECK_NOTHROW(m.update());
    CHECK(a.get_state_reference().cell.coordinates == Coordinates{1,1});
    CHECK(b.get_state_reference().cell.coordinates == Coordinates{1,1});
    CHECK_NOTHROW(m.update());
    CHECK(a.get_state_reference().cell.coordinates == Coordinates{2,1});
    CHECK(b.get_state_reference().cell.coordinates == Coordinates{1,1});
    m.state=s;
    CHECK_NOTHROW(m.update());
    CHECK(a.get_state_reference().cell.coordinates == Coordinates{1,1});
    CHECK(b.get_state_reference().cell.coordinates == Coordinates{1,1});
    cout << m.state << endl;
//    CHECK_NOTHROW(m.update());
//    CHECK(a.get_state_reference().cell.coordinates == Coordinates{2,1});
//    CHECK(b.get_state_reference().cell.coordinates == Coordinates{1,1});
//    CHECK_NOTHROW(m.update());
//    CHECK(a.get_state_reference().cell.coordinates == Coordinates{2,1});
//    CHECK(b.get_state_reference().cell.coordinates == Coordinates{2,1});
//    CHECK_NOTHROW(m.update());
//    CHECK(a.get_state_reference().cell.coordinates == Coordinates{3,1});
//    CHECK(b.get_state_reference().cell.coordinates == Coordinates{2,1});
//    CHECK_NOTHROW(m.update());
//    CHECK(a.get_state_reference().cell.coordinates == Coordinates{3,1});
//    CHECK(b.get_state_reference().cell.coordinates == Coordinates{3,1});
//    CHECK_NOTHROW(m.end_episode());
//    CHECK_THROWS(m.update());
//    CHECK_THROWS(m.end_episode());
}*/