#include<catch.h>
#include<cell_world.h>
#include<iostream>

using namespace cell_world;
using namespace std;

struct Stateless_test_Agent : Stateless_agent {
    Stateless_test_Agent(unsigned int , Cell &c): cell(c){

    };
    Cell cell;
    unsigned int index;

    const Cell &start_episode() override{
        return cell;
    };
    Agent_status_code update_state(const Model_public_state &) override{
        return Agent_status_code::Running;
    };
    Move get_move(const Model_public_state &) override{
        return {1,0};
    };
    void end_episode(const Model_public_state &) override{
    };
};
TEST_CASE("Model status")
{
    World w;
    Cell c0({0,1},{0,1},false);
    Cell c1({1,1},{1,1},false);
    Cell c2({2,1},{2,1},false);
    Cell c3({3,1},{3,1},false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    Cell_group cg = w.create_cell_group();
    Model m(cg);
    Stateless_test_Agent a(1, c0),b(2, c0);
    CHECK_THROWS(m.update());
    CHECK_THROWS(m.start_episode());
    m.add_agent(a);
    m.add_agent(b);
    CHECK_NOTHROW(m.start_episode());
    CHECK(a.public_state().cell.coordinates == Coordinates{0, 1});
    CHECK(b.public_state().cell.coordinates == Coordinates{0, 1});
    CHECK_THROWS(m.start_episode());
    CHECK_NOTHROW(m.update());
    CHECK(a.public_state().cell.coordinates == Coordinates{1, 1});
    CHECK(b.public_state().cell.coordinates == Coordinates{0, 1});
    CHECK_NOTHROW(m.update());
    CHECK(a.public_state().cell.coordinates == Coordinates{1, 1});
    CHECK(b.public_state().cell.coordinates == Coordinates{1, 1});
    CHECK_NOTHROW(m.update());
    CHECK(a.public_state().cell.coordinates == Coordinates{2, 1});
    CHECK(b.public_state().cell.coordinates == Coordinates{1, 1});
    CHECK_NOTHROW(m.update());
    CHECK(a.public_state().cell.coordinates == Coordinates{2, 1});
    CHECK(b.public_state().cell.coordinates == Coordinates{2, 1});
    CHECK_NOTHROW(m.update());
    CHECK(a.public_state().cell.coordinates == Coordinates{3, 1});
    CHECK(b.public_state().cell.coordinates == Coordinates{2, 1});
    CHECK_NOTHROW(m.update());
    CHECK(a.public_state().cell.coordinates == Coordinates{3, 1});
    CHECK(b.public_state().cell.coordinates == Coordinates{3, 1});
    CHECK_NOTHROW(m.end_episode());
    CHECK_THROWS(m.update());
    CHECK_THROWS(m.end_episode());
}


TEST_CASE("Model reset")
{
    World w;
    Cell c0({0,1},{0,1},false);
    Cell c1({1,1},{1,1},false);
    Cell c2({2,1},{2,1},false);
    Cell c3({3,1},{3,1},false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    Cell_group cg = w.create_cell_group();
    Model m(cg);
    Stateless_test_Agent a(1, c0),b(2, c0);
    CHECK_THROWS(m.update());
    CHECK_THROWS(m.start_episode());
    m.add_agent(a);
    m.add_agent(b);
    CHECK_NOTHROW(m.start_episode());
    CHECK(a.public_state().cell.coordinates == Coordinates{0, 1});
    CHECK(b.public_state().cell.coordinates == Coordinates{0, 1});
    CHECK_THROWS(m.start_episode());
    CHECK_NOTHROW(m.update());
    CHECK(a.public_state().cell.coordinates == Coordinates{1, 1});
    Model_state s = m.get_state();
    CHECK_NOTHROW(m.update());
    CHECK(b.public_state().cell.coordinates == Coordinates{1, 1});
    CHECK_NOTHROW(m.update());
    CHECK(a.public_state().cell.coordinates == Coordinates{2, 1});
    m.set_public_state(s.public_state);
    CHECK_NOTHROW(m.update());
    CHECK(b.public_state().cell.coordinates == Coordinates{1, 1});
    CHECK_NOTHROW(m.update());
    CHECK(a.public_state().cell.coordinates == Coordinates{2, 1});
    CHECK_NOTHROW(m.update());
    CHECK(b.public_state().cell.coordinates == Coordinates{2, 1});
    CHECK_NOTHROW(m.update());
    CHECK(a.public_state().cell.coordinates == Coordinates{3, 1});
    CHECK_NOTHROW(m.update());
    CHECK(b.public_state().cell.coordinates == Coordinates{3, 1});
    m.set_state(s);
    CHECK_NOTHROW(m.update());
    CHECK(b.public_state().cell.coordinates == Coordinates{1, 1});
    CHECK_NOTHROW(m.update());
    CHECK(a.public_state().cell.coordinates == Coordinates{2, 1});
    CHECK_NOTHROW(m.update());
    CHECK(b.public_state().cell.coordinates == Coordinates{2, 1});
    CHECK_NOTHROW(m.update());
    CHECK(a.public_state().cell.coordinates == Coordinates{3, 1});
    CHECK_NOTHROW(m.update());
    CHECK(b.public_state().cell.coordinates == Coordinates{3, 1});
    CHECK_NOTHROW(m.end_episode());
    CHECK_THROWS(m.update());
    CHECK_THROWS(m.end_episode());
    auto oa = &m.state.public_state.agents_state[0];
    auto ob = &m.state.public_state.agents_state[1];
    m.restart_episode();
    CHECK( oa == &m.state.public_state.agents_state[0]);
    CHECK( ob == &m.state.public_state.agents_state[1]);
    CHECK(a.public_state().cell.coordinates == Coordinates{0, 1});
    CHECK(b.public_state().cell.coordinates == Coordinates{0, 1});
}

struct Test_state : json_cpp::Json_object {
    Coordinates coordinates;
    Json_object_members({
        Add_member(coordinates);
    })
};

struct Stateful_test_Agent : Stateful_agent<Test_state> {
    Stateful_test_Agent(unsigned int , Cell &c): cell(c){
    };
    Cell cell;
    unsigned int index;

    const Cell &start_episode() override{
        return cell;
    };
    Agent_status_code update_state(const Model_public_state &) override{
        internal_state().coordinates = public_state().cell.coordinates;
        return Agent_status_code::Running;
    };
    Move get_move(const Model_public_state &) override{
        return {1,0};
    };
    void end_episode(const Model_public_state &) override{
    };
};

TEST_CASE("Model statefull reset")
{
    World w;
    Cell c0({0,1},{0,1},false);
    Cell c1({1,1},{1,1},false);
    Cell c2({2,1},{2,1},false);
    Cell c3({3,1},{3,1},false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    Cell_group cg = w.create_cell_group();
    Model m(cg);
    Stateful_test_Agent a(1, c0),b(2, c0);
    CHECK_THROWS(m.update());
    CHECK_THROWS(m.start_episode());
    m.add_agent(a);
    m.add_agent(b);
    CHECK_NOTHROW(m.start_episode());
    CHECK(a.public_state().cell.coordinates == Coordinates{0, 1});
    CHECK(b.public_state().cell.coordinates == Coordinates{0, 1});
    CHECK(a.internal_state().coordinates == Coordinates{0, 1});
    CHECK(b.internal_state().coordinates == Coordinates{0, 1});
    CHECK_THROWS(m.start_episode());
    CHECK_NOTHROW(m.update());
    CHECK(a.public_state().cell.coordinates == Coordinates{1, 1});
    CHECK(a.internal_state().coordinates == Coordinates{1, 1});
    Model_state s = m.get_state();
    CHECK_NOTHROW(m.update());
    CHECK(b.public_state().cell.coordinates == Coordinates{1, 1});
    CHECK(b.internal_state().coordinates == Coordinates{1, 1});
    CHECK_NOTHROW(m.update());
    CHECK(a.public_state().cell.coordinates == Coordinates{2, 1});
    CHECK(a.internal_state().coordinates == Coordinates{2, 1});
    m.set_public_state(s.public_state);
    CHECK_NOTHROW(m.update());
    CHECK(b.public_state().cell.coordinates == Coordinates{1, 1});
    CHECK(b.internal_state().coordinates == Coordinates{1, 1});
    CHECK_NOTHROW(m.update());
    CHECK(a.public_state().cell.coordinates == Coordinates{2, 1});
    CHECK(a.internal_state().coordinates == Coordinates{2, 1});
    CHECK_NOTHROW(m.update());
    CHECK(b.public_state().cell.coordinates == Coordinates{2, 1});
    CHECK(b.internal_state().coordinates == Coordinates{2, 1});
    CHECK_NOTHROW(m.update());
    CHECK(a.public_state().cell.coordinates == Coordinates{3, 1});
    CHECK(a.internal_state().coordinates == Coordinates{3, 1});
    CHECK_NOTHROW(m.update());
    CHECK(b.public_state().cell.coordinates == Coordinates{3, 1});
    CHECK(b.internal_state().coordinates == Coordinates{3, 1});
    Model m2 (cg);
    m2.add_agent(a);
    m2.add_agent(b);
    m2.start_episode();
    m2.set_state(s);
    CHECK_NOTHROW(m2.update());
    CHECK(b.public_state().cell.coordinates == Coordinates{1, 1});
    CHECK_NOTHROW(m2.update());
    CHECK(a.public_state().cell.coordinates == Coordinates{2, 1});
    CHECK_NOTHROW(m2.update());
    CHECK(b.public_state().cell.coordinates == Coordinates{2, 1});
    CHECK_NOTHROW(m2.update());
    CHECK(a.public_state().cell.coordinates == Coordinates{3, 1});
    CHECK_NOTHROW(m2.update());
    CHECK(b.public_state().cell.coordinates == Coordinates{3, 1});
    CHECK_NOTHROW(m2.end_episode());
    CHECK_THROWS(m2.update());
    CHECK_THROWS(m2.end_episode());

}
