#include<catch.h>
#include<cell_world.h>

using namespace cell_world;
using namespace std;

TEST_CASE("cell>>1")
{
    string s = "[50,1,[4,5],[41.5,-5.324],1, 35.4, 103, [-1,-5]]";
    stringstream s_stream(s);
    Cell c0;
    s_stream >> c0;
    CHECK(c0.id==50);
    CHECK(c0.cell_type==Square);
    CHECK(c0.coordinates==Coordinates{4,5});
    CHECK(c0.location==Location{41.5,-5.324});
    CHECK(c0.occluded);
    CHECK(c0.value==35.4);
    CHECK(c0.icon == Icon::Custom_icon_3);
    CHECK(c0.direction==Coordinates{-1,-5});
}

TEST_CASE("cell>>2")
{
    string s = "[50,1,{\"x\":4,\"y\":5},{\"y\":-5.324, \"x\":41.5},1, 35.4, 103, [-1,-5]]";
    stringstream s_stream(s);
    Cell c0;
    s_stream >> c0;
    CHECK(c0.id==50);
    CHECK(c0.cell_type==Square);
    CHECK(c0.coordinates==Coordinates{4,5});
    CHECK(c0.location==Location{41.5,-5.324});
    CHECK(c0.occluded);
    CHECK(c0.value==35.4);
    CHECK(c0.icon == Icon::Custom_icon_3);
    CHECK(c0.direction==Coordinates{-1,-5});
}

TEST_CASE("cell>>3")
{
    string s = "{\"type\":1, \"id\":50,\"coordinates\": {\"x\":4,\"y\":5}, \"location\":{\"y\":-5.324, \"x\":41.5}, \"occluded\": 1, \"value\": 35.4, \"icon\": 103, \"direction\" :[-1,-5]}";
    stringstream s_stream(s);
    Cell c0;
    s_stream >> c0;
    CHECK(c0.id==50);
    CHECK(c0.cell_type==Square);
    CHECK(c0.coordinates==Coordinates{4,5});
    CHECK(c0.location==Location{41.5,-5.324});
    CHECK(c0.occluded);
    CHECK(c0.value==35.4);
    CHECK(c0.icon == Icon::Custom_icon_3);
    CHECK(c0.direction==Coordinates{-1,-5});
}
