#include<catch.h>
#include<cell_world.h>
#include<iostream>

using namespace cell_world;
using namespace std;
TEST_CASE("World")
{
    World w;
    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0}]" >> w.connection_pattern;
    Cell c1({0,1},{1,1},false);
    Cell c2({1,1},{1,1},false);
    Cell c3({2,1},{1,1},false);
    Cell c4({3,1},{1,1},false);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    w.add(c4);
    CHECK(w.size()==4);
}

TEST_CASE("World connections")
{
    World w;
    string s ("{\"connection_pattern\":[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0}],\"cells\":[{\"id\":0,\"cell_type\":0,\"coordinates\":{\"x\":0,\"y\":1},\"location\":{\"x\":1,\"y\":1},\"occluded\":0},{\"id\":1,\"cell_type\":0,\"coordinates\":{\"x\":1,\"y\":1},\"location\":{\"x\":1,\"y\":1},\"occluded\":0},{\"id\":2,\"cell_type\":0,\"coordinates\":{\"x\":2,\"y\":1},\"location\":{\"x\":1,\"y\":1},\"occluded\":0},{\"id\":3,\"cell_type\":0,\"coordinates\":{\"x\":3,\"y\":1},\"location\":{\"x\":1,\"y\":1},\"occluded\":0}]}");
    s >> w;
    CHECK(w.size()==4);
    CHECK(w.connection_pattern.size()==2);
}


TEST_CASE("world>>1")
{
    string s = "{\"connection_pattern\":[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0}],\"cells\":[";
    s+="{\"id\":0,\"cell_type\":1,\"coordinates\":{\"x\":0,\"y\":1},\"location\":{\"x\":0.5,\"y\":1.5},\"occluded\":1},";
    s+="{\"id\":1,\"cell_type\":0,\"coordinates\":{\"x\":1,\"y\":2},\"location\":{\"x\":1.5,\"y\":2.5},\"occluded\":0},";
    s+="{\"id\":2,\"cell_type\":1,\"coordinates\":{\"x\":2,\"y\":3},\"location\":{\"x\":2.5,\"y\":3.5},\"occluded\":1},";
    s+="{\"id\":3,\"cell_type\":1,\"coordinates\":{\"x\":3,\"y\":4},\"location\":{\"x\":3.5,\"y\":4.5},\"occluded\":0}]}";
    stringstream s_stream(s);
    World w;
    s_stream >> w;
    CHECK(w.size()==4);
    CHECK(w.connection_pattern.size()==2);
    auto c0 = w[0];
    CHECK(c0.id==0);
    CHECK(c0.coordinates==Coordinates{0,1});
    CHECK(c0.location==Location{0.5,1.5});
    CHECK(c0.occluded);
    auto c1 = w[1];
    CHECK(c1.id==1);
    CHECK(c1.coordinates==Coordinates{1,2});
    CHECK(c1.location==Location{1.5,2.5});
    CHECK(!c1.occluded);
    auto c2 = w[2];
    CHECK(c2.id==2);
    CHECK(c2.coordinates==Coordinates{2,3});
    CHECK(c2.location==Location{2.5,3.5});
    CHECK(c2.occluded);
    auto c3 = w[3];
    CHECK(c3.id==3);
    CHECK(c3.coordinates==Coordinates{3,4});
    CHECK(c3.location==Location{3.5,4.5});
    CHECK(!c3.occluded);
}


TEST_CASE("from resources"){
//    cout << "from resources" << endl;
//    cout << World::get_from_parameters_name("hexagonal","canonical") << endl;
//    cout << World::get_from_parameters_name("hexagonal","canonical", "10_05") << endl;
}

TEST_CASE("World statistics") {
//    auto world = World::get_from_parameters_name("hexagonal","canonical", "21_05");
//    auto stats = world.get_statistics(5);
//    cout << stats;
//    stats.save("21_05.stats.json");
}