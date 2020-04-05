#include<catch.h>
#include<cell_world.h>

using namespace cell_world;

TEST_CASE("Pattern load from string")
{
    Connection_pattern cp;
    cp.load_from_string("1 1 -1 -1 0 0 ");
    CHECK(cp.pattern[0] == Coordinates({1,1}));
    CHECK(cp.pattern[1] == Coordinates({-1,-1}));
    CHECK(cp.pattern[2] == Coordinates({0,0}));
    CHECK(cp.pattern.size() == 3);
    CHECK(cp.save_to_string() == "1 1 -1 -1 0 0 ");
}
