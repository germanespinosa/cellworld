#include<catch.h>
#include<cell_world.h>
#include<iostream>
using namespace cell_world;
using namespace std;
TEST_CASE("World")
{
    World w("test");
    Cell c1(Circle, 0,{0,1},{1,1},0,false);
    Cell c2(Circle, 1,{1,1},{1,1},0,false);
    Cell c3(Circle, 2,{2,1},{1,1},0,false);
    Cell c4(Circle, 3,{3,1},{1,1},0,false);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    w.add(c4);
    w.connection_pattern = Connection_pattern({{-1,0},{1,0}});
    Graph wc = w.create_graph();
    CHECK(w.size()==4);
}

TEST_CASE("World connections")
{
    World w("test");
    Cell c0(Circle, 0,{0,1},{1,1},0,false);
    Cell c1(Circle, 1,{1,1},{1,1},0,false);
    Cell c2(Circle, 2,{2,1},{1,1},0,false);
    Cell c3(Circle, 3,{3,1},{1,1},0,false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    cout << w << endl;
    w.connection_pattern = Connection_pattern({{-1,0},{1,0}});
    Graph wc = w.create_graph();
    CHECK(w.size()==4);
    CHECK(wc[0].size()==1);
    CHECK(wc[0][0].id==1);
    CHECK(wc[1].size()==2);
    CHECK(wc[1][0].id==0);
    CHECK(wc[1][1].id==2);
    CHECK(wc[2].size()==2);
    CHECK(wc[2][0].id==1);
    CHECK(wc[2][1].id==3);
    CHECK(wc[3].size()==1);
    CHECK(wc[3][0].id==2);
}
