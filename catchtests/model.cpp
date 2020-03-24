#include<catch.h>
#include<cell_world.h>
#include<iostream>
using namespace cell_world;
using namespace std;

TEST_CASE("Model status")
{
    World w("test");
    Cell c0(Circle, 0,{0,1},{1,1},0,false);
    Cell c1(Circle, 1,{1,1},{1,1},0,false);
    Cell c2(Circle, 2,{2,1},{1,1},0,false);
    Cell c3(Circle, 3,{3,1},{1,1},0,false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    Cell_group cg = w.create_cell_group();
    Model m(cg);
    CHECK_THROWS(m.update());
    m.start_episode();
    CHECK_THROWS(m.start_episode());
    CHECK_NOTHROW(m.update());
    m.end_episode();
    CHECK_THROWS(m.update());
    CHECK_THROWS(m.end_episode());
    CHECK_NOTHROW(m.start_episode());
}