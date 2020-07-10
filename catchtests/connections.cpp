#include<catch.h>
#include<cell_world.h>

using namespace cell_world;

TEST_CASE("Pattern load from string")
{
    Connection_pattern cp;
    cp.load_from_string("1 1 -1 -1 0 0 ");
    CHECK(cp[0] == Coordinates({1,1}));
    CHECK(cp[1] == Coordinates({-1,-1}));
    CHECK(cp[2] == Coordinates({0,0}));
    CHECK(cp.size() == 3);
    CHECK(cp.save_to_string() == "1 1 -1 -1 0 0 ");
}

TEST_CASE("Connection_pattern =")
{
    Connection_pattern cp;
    cp ="(1, 1);(-1, -1);(0, 0)";
    CHECK(cp[0] == Coordinates({1,1}));
    CHECK(cp[1] == Coordinates({-1,-1}));
    CHECK(cp[2] == Coordinates({0,0}));
    CHECK(cp.size() == 3);
    CHECK(cp.save_to_string() == "1 1 -1 -1 0 0 ");
}

