#include<catch.h>
#include<cellworld.h>

using namespace cell_world;

TEST_CASE("cell_group")
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
    Cell_group cg(w);
    cg.add(0);
    cg.add(1);
    cg.add(2);
    cg.add(3);
    Cell_group p = cg;
    CHECK(cg.size() == p.size());
    CHECK(cg[0] == p[0]);
    CHECK(cg[1] == p[1]);
    CHECK(cg[2] == p[2]);
    CHECK(cg[3] == p[3]);
    Cell_group t(w);
    t = p;
    CHECK(cg.size() == t.size());
    CHECK(c0 == t[0]);
    CHECK(c1 == t[1]);
    CHECK(c2 == t[2]);
    CHECK(c3 == t[3]);
}