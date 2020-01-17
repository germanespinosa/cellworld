#include<catch.h>
#include<cell_world.h>

using namespace cell_world;

TEST_CASE("World")
{
    World w("test");
    Cell c1(0,{0,1},{1,1},0,false);
    Cell c2(1,{1,1},{1,1},0,false);
    Cell c3(2,{2,1},{1,1},0,false);
    Cell c4(3,{3,1},{1,1},0,false);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    w.add(c4);
    Connections wc( w.create_cell_group(),Connection_pattern({{-1,0},{1,0}}));
    CHECK(w.size()==4);
}

TEST_CASE("World connections")
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
    Connections wc(w.create_cell_group(),Connection_pattern({{-1,0},{1,0}}));
    CHECK(w.size()==4);
    CHECK(wc[0].connections.size()==1);
    CHECK(wc[0].connections[0].id==1);
    CHECK(wc[1].connections.size()==2);
    CHECK(wc[1].connections[0].id==0);
    CHECK(wc[1].connections[1].id==2);
    CHECK(wc[2].connections.size()==2);
    CHECK(wc[2].connections[0].id==1);
    CHECK(wc[2].connections[1].id==3);
    CHECK(wc[3].connections.size()==1);
    CHECK(wc[3].connections[0].id==2);
    wc.save("worldtest");
}
