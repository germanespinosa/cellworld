#include<catch.h>
#include<cell_world.h>

using namespace cell_world;

TEST_CASE("Connections shortest path")
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
    Connections c(w.create_cell_group(), Connection_pattern({{1,0},{-1,0}}));
    auto sp = c.shortest_path(c0,c3);
    CHECK(sp.size()==2);
    CHECK(sp[0]==c2);
    CHECK(sp[1]==c1);
    c.save("test");
}

TEST_CASE("Pattern load from string")
{
    Connection_pattern cp;
    cp.load_from_string("1 1 -1 -1 0 0 ");
    CHECK(cp.pattern[0] == Coordinates({1,1}));
    CHECK(cp.pattern[1] == Coordinates({-1,-1}));
    CHECK(cp.pattern[2] == Coordinates({0,0}));
    CHECK(cp.pattern.size() == 3);
    CHECK(cp.save_to_string() == "1 1 -1 -1 0 0 ");
}