#include<catch.h>
#include<cell_world.h>

using namespace cell_world;
using namespace std;
TEST_CASE("Pattern load from string")
{
    Connection_pattern cp;
    "[{\"x\":1, \"y\":1},{\"x\":-1,\"y\":-1},{\"x\":0,\"y\":0}]" >> cp;
    CHECK(cp[0] == Coordinates({1,1}));
    CHECK(cp[1] == Coordinates({-1,-1}));
    CHECK(cp[2] == Coordinates({0,0}));
    CHECK(cp.size() == 3);
    string s;
    s << cp;
    CHECK(s == "[{\"x\":1,\"y\":1},{\"x\":-1,\"y\":-1},{\"x\":0,\"y\":0}]");
}
