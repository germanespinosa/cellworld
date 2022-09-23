#include<catch.h>
#include<cell_world.h>
#include<iostream>

using namespace cell_world;
using namespace std;

TEST_CASE("Map") {
    World w;
    Cell c0({0,1},{1,1},false);
    Cell c1({1,1},{1,1},false);
    Cell c2({2,1},{1,1},false);
    Cell c3({3,1},{1,1},false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    //cout << endl;
    Cell_group cg = w.create_cell_group();
    Map m(cg);
}