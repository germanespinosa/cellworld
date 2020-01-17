#include<catch.h>
#include<cell_world.h>

using namespace cell_world;

TEST_CASE("Location l==l")
{
    Location l0{100,100};
    Location l1{100,100};
    CHECK( l0 == l1 );
}

TEST_CASE("Location l+l")
{
    Location l0{100,100};
    Location l1{100,100};
    Location l2{200,200};
    CHECK( l0 + l1 == l2);
}

TEST_CASE("Location l-l")
{
    Location l0{100,100};
    Location l1{75,75};
    Location l2{25,25};
    CHECK( l0 - l1 == l2);
}

TEST_CASE("Location mod")
{
    Location l0{3,4};
    CHECK( l0.mod() == 5);
}


TEST_CASE("Location dist")
{
    Location l0{103,104};
    Location l1{100,100};
    CHECK( l0.dist(l1) == 5);
    CHECK( l1.dist(l0) == 5);
}

