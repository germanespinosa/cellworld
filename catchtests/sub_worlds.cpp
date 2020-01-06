#include<catch.h>
#include<cellworld.h>

using namespace cellworld;

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
    Connections wc;
    w.get_connections(wc,{{-1,0},{1,0}});
    Sub_worlds sw;
    Cell_group cg(w);
    cg.add(0);
    cg.add(2);
    cg.add(4);
    sw.reset(w,cg,wc);
    sw.reset_connections();
    CHECK(wc.size()==5);
    CHECK(wc[0].size()==1);
    CHECK(wc[1].size()==2);
    CHECK(wc[2].size()==2);
    CHECK(wc[3].size()==2);
    CHECK(wc[4].size()==1);
    CHECK(sw.size() == 2);
    CHECK(sw.gate_by_cell_id(0).is_connected(0));
    CHECK_FALSE(sw.gate_by_cell_id(0).is_connected(1));
    CHECK(sw.gate_by_cell_id(2).is_connected(0));
    CHECK(sw.gate_by_cell_id(2).is_connected(1));
    CHECK_FALSE(sw.gate_by_cell_id(4).is_connected(0));
    CHECK(sw.gate_by_cell_id(4).is_connected(1));
    CHECK(sw.gate_by_cell_id(2).gate_connections.size()==2);
    CHECK(sw.gate_by_cell_id(2).gate_connections[0].gate_id==0);
    CHECK(sw.gate_by_cell_id(2).gate_connections[1].gate_id==4);
}