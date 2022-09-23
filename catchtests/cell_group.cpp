#include<catch.h>
#include<cell_world.h>

using namespace std;
using namespace cell_world;

TEST_CASE("cell_group")
{
    World w;
    Cell c0({0,1},{1,1},false);
    Cell c1({1,1},{1,1},false);
    Cell c2({2,1},{1,1},false);
    Cell c3({3,1},{1,1},false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    Cell_group cg = w.create_cell_group();
    string s;
    s << cg;
    CHECK(s == "[0,1,2,3]");
    Cell_group cg2;
    cg2.add(w[0]);
    cg2.add(w[1]);
    cg2.add(w[2]);
    cg2.add(w[3]);
    string s2;
    s2 << cg2;
    CHECK(s2 == "[0,1,2,3]");
}

TEST_CASE("cell_group 2")
{
    World w;
    Cell c0({0,1},{1,1},false);
    Cell c1({1,1},{1,1},false);
    Cell c2({2,1},{1,1},false);
    Cell c3({3,1},{1,1},false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    Cell_group cg;
    cg.add(w[0]);
    cg.add(w[1]);
    cg.add(w[2]);
    CHECK(cg.to_json() == "[0,1,2]");
    Cell_group cg2;
    cg2.add(w[1]);
    cg2.add(w[2]);
    cg2.add(w[3]);
    CHECK(cg2.to_json() == "[1,2,3]");
    string s;
    s << (cg & cg2);
    CHECK(s == "[1,2]");
}
