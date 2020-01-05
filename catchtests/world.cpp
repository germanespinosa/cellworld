#include<catch.h>
#include<cellworld.h>

using namespace cellworld;

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
    Connections wc;
    w.get_connections(wc,{{-1,0},{1,0}});
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
    Connections wc;
    w.get_connections(wc,{{-1,0},{1,0}});
    CHECK(w.size()==4);
    CHECK(wc[0].size()==1);
    CHECK(wc[0][0]==1);
    CHECK(wc[1].size()==2);
    CHECK(wc[1][0]==0);
    CHECK(wc[1][1]==2);
    CHECK(wc[2].size()==2);
    CHECK(wc[2][0]==1);
    CHECK(wc[2][1]==3);
    CHECK(wc[3].size()==1);
    CHECK(wc[3][0]==2);
    wc.save("worldtest");
}