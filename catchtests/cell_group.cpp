#include<catch.h>
#include<cell_world.h>

using namespace std;
using namespace cell_world;

TEST_CASE("cell_group")
{
    World w("test");
    Cell c0(Circle, {0,1},{1,1},0,false);
    Cell c1(Circle, {1,1},{1,1},0,false);
    Cell c2(Circle, {2,1},{1,1},0,false);
    Cell c3(Circle, {3,1},{1,1},0,false);
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

    cg2.remove(c2);
    string s3;
    s3 << cg2;
    CHECK(s3 == "[0,1,3]");
}

TEST_CASE("cell_group 2")
{
    World w("test");
    Cell c0(Circle, {0,1},{1,1},0,false);
    Cell c1(Circle, {1,1},{1,1},0,false);
    Cell c2(Circle, {2,1},{1,1},0,false);
    Cell c3(Circle, {3,1},{1,1},0,false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    Cell_group cg;
    cg.add(c0);
    cg.add(c1);
    cg.add(c2);
    Cell_group cg2;
    cg2.add(c1);
    cg2.add(c2);
    cg2.add(c3);
    string s;
    s << (cg & cg2);
    CHECK(s == "[1,2]");
}
