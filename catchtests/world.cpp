#include<catch.h>
#include<cell_world.h>
#include<iostream>

using namespace cell_world;
using namespace std;
TEST_CASE("World")
{
    World w("test");
    w.connection_pattern = Connection_pattern({{-1,0},{1,0}});
    Cell c1(Circle, {0,1},{1,1},0,false);
    Cell c2(Circle, {1,1},{1,1},0,false);
    Cell c3(Circle, {2,1},{1,1},0,false);
    Cell c4(Circle, {3,1},{1,1},0,false);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    w.add(c4);
    CHECK(w.size()==4);
}

TEST_CASE("World connections")
{
    World w;
    string s ("{\"name\":\"test\",\"connection_pattern\":[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0}],\"cells\":[{\"id\":0,\"cell_type\":0,\"coordinates\":{\"x\":0,\"y\":1},\"location\":{\"x\":1,\"y\":1},\"occluded\":0,\"value\":0,\"direction\":{\"x\":0,\"y\":0}},{\"id\":1,\"cell_type\":0,\"coordinates\":{\"x\":1,\"y\":1},\"location\":{\"x\":1,\"y\":1},\"occluded\":0,\"value\":0,\"direction\":{\"x\":0,\"y\":0}},{\"id\":2,\"cell_type\":0,\"coordinates\":{\"x\":2,\"y\":1},\"location\":{\"x\":1,\"y\":1},\"occluded\":0,\"value\":0,\"direction\":{\"x\":0,\"y\":0}},{\"id\":3,\"cell_type\":0,\"coordinates\":{\"x\":3,\"y\":1},\"location\":{\"x\":1,\"y\":1},\"occluded\":0,\"value\":0,\"direction\":{\"x\":0,\"y\":0}}]}");
    s >> w;
    CHECK(w.name=="test");
    CHECK(w.size()==4);
    CHECK(w.connection_pattern.size()==2);
}


TEST_CASE("world>>1")
{
    string s ("{\"name\":\"w\",\"connection_pattern\":[{\"x\":-1,\"y\":1},{\"x\":2,\"y\":3}],\"cells\":["
              "{\"id\":0,\"cell_type\":0,\"coordinates\":{\"x\":0,\"y\":1},\"location\":{\"x\":1,\"y\":1},\"occluded\":0,\"value\":0,\"direction\":{\"x\":0,\"y\":0}},"
              "{\"id\":1,\"cell_type\":0,\"coordinates\":{\"x\":1,\"y\":1},\"location\":{\"x\":1,\"y\":1},\"occluded\":0,\"value\":0,\"direction\":{\"x\":0,\"y\":0}},"
              "{\"id\":2,\"cell_type\":0,\"coordinates\":{\"x\":2,\"y\":1},\"location\":{\"x\":1,\"y\":1},\"occluded\":0,\"value\":0,\"direction\":{\"x\":0,\"y\":0}},{\"id\":3,\"cell_type\":0,\"coordinates\":{\"x\":3,\"y\":1},\"location\":{\"x\":1,\"y\":1},\"occluded\":0,\"value\":0,\"direction\":{\"x\":0,\"y\":0}}]}");
    string s = "[\"w\",[[-1,1],[2,3]],[";
    s+="[0,1,[4,5],[41.5,-5.324],1, 35.4, 103, [-1,-5]],";
    s+="[1,0,[5,5],[41.5,-5.324],1, 35.4, 103, [-1,-5]],";
    s+="[2,1,[6,5],[41.5,-5.324],1, 35.4, 103, [-1,-5]]";
    s+="]]";
    stringstream s_stream(s);
    World w("w");
    s_stream >> w;
    CHECK(w.name=="w");
    CHECK(w.size()==3);
    CHECK(w.connection_pattern.size()==2);
    auto c0 = w[0];
    CHECK(c0.id==0);
    CHECK(c0.cell_type==Square);
    CHECK(c0.coordinates==Coordinates{4,5});
    CHECK(c0.location==Location{41.5,-5.324});
    CHECK(c0.occluded);
    CHECK(c0.value==35.4);
    CHECK(c0.icon == Icon::Custom_icon_3);
    CHECK(c0.direction==Coordinates{-1,-5});
    auto c1 = w[1];
    CHECK(c1.id==1);
    CHECK(c1.cell_type==Circle);
    CHECK(c1.coordinates==Coordinates{5,5});
    CHECK(c1.location==Location{41.5,-5.324});
    CHECK(c1.occluded);
    CHECK(c1.value==35.4);
    CHECK(c1.icon == Icon::Custom_icon_3);
    CHECK(c1.direction==Coordinates{-1,-5});
    auto c2 = w[2];
    CHECK(c2.id==2);
    CHECK(c2.cell_type==Square);
    CHECK(c2.coordinates==Coordinates{6,5});
    CHECK(c2.location==Location{41.5,-5.324});
    CHECK(c2.occluded);
    CHECK(c2.value==35.4);
    CHECK(c2.icon == Icon::Custom_icon_3);
    CHECK(c2.direction==Coordinates{-1,-5});
}

/*
TEST_CASE("world>>2")
{
    string s = "{\"name\":\"w\",\"cells\":[";
    s+="[0,1,[4,5],[41.5,-5.324],1, 35.4, 103, [-1,-5]],";
    s+="[1,0,[5,5],[41.5,-5.324],1, 35.4, 103, [-1,-5]],";
    s+="[2,1,[6,5],[41.5,-5.324],1, 35.4, 103, [-1,-5]]";
    s+="],\"connection_pattern\":[[1,1],[2,3]]}";
    stringstream s_stream(s);
    World w("w");
    s_stream >> w;
    CHECK(w.name=="w");
    CHECK(w.size()==3);
    CHECK(w.connection_pattern.size()==2);
}
*/