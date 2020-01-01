#include<catch.h>
#include<cellworld.h>

using namespace cellworld;

TEST_CASE("Connection")
{
    Connection c;
    CHECK(c.remove(0) == false);
    CHECK(c.is_connected(0) == false);
    CHECK(c.add(0) == true);
    CHECK(c.add(0) == false);
    CHECK(c.is_connected(0) == true);
    CHECK(c.remove(0) == true);
    CHECK(c.is_connected(0) == false);
}

TEST_CASE("Connections")
{
    Connections c;
    CHECK(c.add(0,1) == true);
    CHECK(c.add(0,1) == false);
    CHECK(c.add(0,2) == true);
    CHECK(c.add(0,2) == false);
    CHECK(c.add(100,200) == true);
    CHECK(c.add(100,200) == false);
    CHECK(c[0].is_connected(1) == true);
    CHECK(c[1].is_connected(0) == false);
    CHECK(c[0].size() == 2);
    CHECK(c[100].is_connected(200) == true);
    CHECK(c[100].size() == 1);
    c.save("test");
}