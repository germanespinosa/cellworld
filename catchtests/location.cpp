#include<catch.h>
#include<cell_world.h>

using namespace cell_world;
using namespace std;

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

TEST_CASE("location>>1")
{
    string s = "[41.5,-5.324]";
    stringstream s_stream(s);
    Location c0;
    Location c1 {41.5,-5.324};
    s_stream >> c0;
    CHECK(c0==c1);
}


TEST_CASE("location>>5")
{
    string s = " [ 41  -53 ] ";
    stringstream s_stream(s);
    Location c0;
    CHECK_THROWS(s_stream >> c0);
}
