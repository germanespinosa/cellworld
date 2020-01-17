#include<catch.h>
#include<cell_world.h>

using namespace cell_world;

TEST_CASE("Connections")
{
    World w("test");
    Cell c0(0,{0,1},{1,1},0,false);
    Cell c1(1,{1,1},{1,1},0,false);
    Cell c2(2,{2,1},{1,1},0,false);
    Cell c3(3,{3,1},{1,1},0,false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    Connections c(w.create_cell_group());
    CHECK(c.add(c0,c1) == true);
    CHECK(c[c0].connections.contains(c1) == true);
    CHECK(c.add(c0,c1) == false);
    CHECK(c.add(c0,c2) == true);
    CHECK(c[c0].connections.contains(c2) == true);
    CHECK(c.add(c0,c2) == false);
    CHECK(c.add(c1,c2) == true);
    CHECK(c[c1].connections.contains(c2) == true);
    CHECK(c.add(c1,c2) == false);
    c.save("test");
}