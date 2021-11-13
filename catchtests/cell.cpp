#include<catch.h>
#include<cell_world.h>
#include<sstream>

using namespace cell_world;
using namespace std;

TEST_CASE("cell>>1")
{
    string s = "{\"id\":50,\"cell_type\":1,\"coordinates\":{\"x\":4,\"y\":5},\"location\":{\"x\":41.5,\"y\":-5.324},\"occluded\":1}";
    stringstream s_stream(s);
    Cell c0;
    s_stream >> c0;
    CHECK(c0.id==50);
    CHECK(c0.coordinates==Coordinates{4,5});
    CHECK(c0.location==Location{41.5,-5.324});
    CHECK(c0.occluded);
}

TEST_CASE("cell>>2")
{
    string s = "{\"id\":50,\"type\":1,\"coordinates\":{\"x\":4,\"y\":5},\"location\":{\"x\":41.5,\"y\":-5.324},\"occluded\":1}";
    stringstream s_stream(s);
    Cell c0;
    CHECK_THROWS(s_stream >> c0);
}

TEST_CASE("cell bool")
{
    Cell c;
    CHECK(c);
    c = Cell::ghost_cell();
    CHECK(!c);
}