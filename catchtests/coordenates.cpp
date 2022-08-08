#include<catch.h>
#include<cell_world.h>
#include <sstream>

using namespace std;
using namespace cell_world;

TEST_CASE("Coordinates c==c")
{
    Coordinates c0{100,100};
    Coordinates c1{100,100};
    CHECK( c0 == c1 );
}

TEST_CASE("Coordinates c=c")
{
    Coordinates c1{100,-100};
    Coordinates c0;
    "{\"x\":100,\"y\":-100}" >> c0;
    CHECK( c0 == c1 );
    "{\"x\":100,\"y\":-100}" >> c1;
    CHECK( c0 == c1 );
}

TEST_CASE("Coordinates c!=c")
{
    Coordinates c0{100,100};
    Coordinates c1{100,20};
    CHECK( c0 != c1 );
    Coordinates c2{100,100};
    Coordinates c3{20,100};
    CHECK( c2 != c3 );
}


TEST_CASE("Coordinates c+c")
{
    Coordinates c0{10,5};
    Coordinates c1{15,30};
    Coordinates c2{25,35};
    CHECK( (c0+c1) == c2 );
}

TEST_CASE("Coordinates c+=c")
{
    Coordinates c0{10,5};
    Coordinates c1{15,30};
    Coordinates c2{25,35};
    c0+=c1;
    CHECK( c0 == c2 );
}

TEST_CASE("Coordinates c-c")
{
    Coordinates c0{25,35};
    Coordinates c1{15,30};
    Coordinates c2{10,5};
    CHECK( (c0-c1) == c2 );
}

TEST_CASE("Coordinates -c")
{
    Coordinates c0{10,5};
    Coordinates c1{-10,-5};
    CHECK( -c0 == c1 );
}

TEST_CASE(">>1")
{
    string s = "{\"x\":3,\"y\":-8} {\"x\":41,\"y\":-5}";
    stringstream s_stream(s);
    Coordinates c0;
    Coordinates c1 {3,-8};
    Coordinates c2 {41,-5};
    s_stream >> c0;
    CHECK(c0==c1);
    s_stream >> c0;
    CHECK(c0==c2);
}

TEST_CASE(">>2")
{
    string s = " {\"x\": 41 , \"y\":-53 }, ";
    stringstream s_stream(s);
    Coordinates c0;
    Coordinates c1 {41,-53};
    s_stream >> c0;
    CHECK(c0==c1);
}

TEST_CASE(">>3")
{
    string s = " {\"x\": 41 , \"y\":-53 , ";
    stringstream s_stream(s);
    Coordinates c0;
    CHECK_THROWS(s_stream >> c0);
}

TEST_CASE(">>4")
{
    string s = " \"x\": 41 , \"y\":-53 , ";
    stringstream s_stream(s);
    Coordinates c0;
    CHECK_THROWS(s_stream >> c0);
}

TEST_CASE(">>5")
{
    string s = " \"x\": 41  \"y\":-53 , ";
    stringstream s_stream(s);
    Coordinates c0;
    CHECK_THROWS(s_stream >> c0);
}

TEST_CASE(">>7")
{
    stringstream s_stream(R"( { "x": 41 , "y": -53 })");
    Coordinates c0;
    Coordinates c1 {41,-53};
    s_stream >> c0;
    CHECK(c0==c1);
}

TEST_CASE("goup>>1")
{
    stringstream s_stream(R"( [{ "x": 41 , "y": -53 }, { "x": 1 , "y": -3 }])");
    Coordinates c0 {41,-53};
    Coordinates c1 {1,-3};
    Coordinates_list cg;
    s_stream >> cg;
    CHECK(cg.size() == 2);
    CHECK(c0==cg[0]);
    CHECK(c1==cg[1]);
}


TEST_CASE(">>8")
{
    string s = " [  { \"x\": 41 , \"y\": -53 }, { \"x\": 1 , \"y\": -3 } , { \"x\": 4 , \"y\": -5 } ] ";
    stringstream s_stream(s);
    Coordinates c0 {41,-53};;
    Coordinates c1 {1,-3};
    Coordinates c2 {4,-5};
    Coordinates_list cg;
    s_stream >> cg;
    CHECK(cg.size()==3);
    CHECK(cg[0]==c0);
    CHECK(cg[1]==c1);
    CHECK(cg[2]==c2);
}

TEST_CASE(">>9")
{
    string s = " [   ] ";
    stringstream s_stream(s);
    Coordinates_list cg;
    s_stream >> cg;
    CHECK(cg.size() == 0);
}
