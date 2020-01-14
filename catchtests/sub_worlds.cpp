#include<catch.h>
#include<cellworld.h>
#include <iostream>
using namespace cell_world;
using namespace std;
TEST_CASE("Sub Worlds")
{
    World w("test");
    Cell c0(0,{0,1},{1,1},0,false);
    Cell c1(1,{1,1},{1,1},0,false);
    Cell c2(2,{2,1},{1,1},0,false);
    Cell c3(3,{3,1},{1,1},0,false);
    Cell c4(4,{4,1},{1,1},0,false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    w.add(c4);
    Cell_group cg1= w.create_cell_group();
    Connections wc (cg1,{{{-1,0},{1,0},{0,-1},{0,1}}});
    Cell_group cg;
    cg.add(w[0]);
    cg.add(w[2]);
    cg.add(w[4]);
    CHECK(wc.size()==5);
    CHECK(wc[0].connections.size()==1);
    CHECK(wc[1].connections.size()==2);
    CHECK(wc[2].connections.size()==2);
    CHECK(wc[3].connections.size()==2);
    CHECK(wc[4].connections.size()==1);

    Sub_worlds sw;
    sw.reset(w.create_cell_group(),cg,wc);
    CHECK(sw.size() == 2);
    CHECK(sw[0].gate_cells.size() == 2);
    CHECK(sw[0].gate_cells[0].id == 0);
    CHECK(sw[0].gate_cells[1].id == 2);
    CHECK(sw[1].gate_cells.size() == 2);
    CHECK(sw[1].gate_cells[0].id == 2);
    CHECK(sw[1].gate_cells[1].id == 4);

    CHECK(sw.gates[0].gate_connections.size() == 1);
    CHECK(sw.gates[1].gate_connections.size() == 2);
    CHECK(sw.gates[0].gate_connections.size() == 1);

}

TEST_CASE("Sub Worlds big") {
    World w("test");
    for (int8_t i = 0; i <=100; i++){
        Cell c0(0, {i, i}, {1, 1}, 0, false);
        w.add(c0);
    }
    Cell_group cg1 = w.create_cell_group();
    Connections wc(cg1, {{{-1, 0},{1,  0}}});
    Sub_worlds sw;
    Cell_group cg;
    cg.add(w[0]);
    cg.add(w[50]);
    cg.add(w[100]);
    sw.reset(w.create_cell_group(), cg, wc);
}