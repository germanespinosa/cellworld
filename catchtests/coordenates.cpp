#include<catch.h>
#include<cell_world.h>

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
    c0 = "100,-100";
    CHECK( c0 == c1 );
    c0 = "(100,-100)";
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
    string s = "[3,-8] [41,-5]";
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
    string s = " [ 41 , -53 ], ";
    stringstream s_stream(s);
    Coordinates c0;
    Coordinates c1 {41,-53};
    s_stream >> c0;
    CHECK(c0==c1);
}

TEST_CASE(">>3")
{
    string s = " [ 41 , -53 , ";
    stringstream s_stream(s);
    Coordinates c0;
    CHECK_THROWS(s_stream >> c0);
}

TEST_CASE(">>4")
{
    string s = "  41 , -53 ] ";
    stringstream s_stream(s);
    Coordinates c0;
    CHECK_THROWS(s_stream >> c0);
}

TEST_CASE(">>5")
{
    string s = " [ 41  -53 ] ";
    stringstream s_stream(s);
    Coordinates c0;
    CHECK_THROWS(s_stream >> c0);
}


TEST_CASE(">>6")
{
    string s = " \"hello\" ";
    stringstream s_stream(s);
    CHECK(read_string(s_stream) == "hello");
}

TEST_CASE(">>7")
{
    string s = " { \"x\": 41 , \"y\": -53 }";
    stringstream s_stream(s);
    Coordinates c0;
    Coordinates c1 {41,-53};
    s_stream >> c0;
    CHECK(c0==c1);
}

