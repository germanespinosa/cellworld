#include<catch.h>
#include<cell_world.h>
#include <sstream>

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
    string s = "{ \"x\":41.5,\"y\":-5.324}";
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

double round(double v, int d){
    return ((int) (v * pow(10, d))) / pow(10, d);
}

TEST_CASE("location atan")
{
    Location c0{-.6,.5};
    Location c1{1, 0};
    double d = c0.dist(c1);
    double t = c0.atan(c1);
    auto c2 = c0.move(t,d);
    CHECK(round(c2.x,5) == c1.x);
    CHECK(round(c2.y,5) == c1.y);
}

TEST_CASE("distance to line") {
    Location l1 ( 10,10);
    Location l2 ( 20,20);
    Location p ( 10,20);
    CHECK(Location(10,20).dist(l1,l2) == Location(9,19).dist(l1,l2));
}


TEST_CASE("intersect") {
    Location l11 ( 10,10);
    Location l12 ( 20,20);

    Location l21 ( 10,20);
    Location l22 ( 20,10);

    //cout << "intersects? " << segments_intersect(l11,l12,l21,l22) << endl;

}
