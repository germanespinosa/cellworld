#include<catch.h>
#include<cellworld.h>

using namespace cellworld;

TEST_CASE("Coordinates c==c")
{
    Coordinates c0{100,100};
    Coordinates c1{100,100};
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
