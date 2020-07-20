#include<catch.h>
#include<cell_world.h>
#include<iostream>

using namespace cell_world;
using namespace std;

TEST_CASE("Map") {
    World w("test");
    Cell c0(Circle, {0,1},{1,1},0,false);
    Cell c1(Circle, {1,1},{1,1},0,false);
    Cell c2(Circle, {2,1},{1,1},0,false);
    Cell c3(Circle, {3,1},{1,1},0,false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    cout << endl;
    Cell_group cg = w.create_cell_group();
    Map m(cg);
}