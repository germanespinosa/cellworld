#include<catch.h>
#include<cellworld.h>

using namespace cell_world;


TEST_CASE("Histogram and Entropy")
{
    CHECK(histogram({0,0,0,1}).size()==2);
    CHECK(histogram({0,0,0,1,2}).size()==3);
    CHECK(histogram({0,0,0,0,0}).size()==1);
    CHECK(histogram({10,10,10,11,12}).size()==3);
    CHECK(histogram({10,10,10,11,12})[0]==3);
    CHECK(histogram({10,10,10,11,12})[1]==1);
    CHECK(histogram({10,10,10,11,12})[2]==1);

    CHECK(abs(entropy(histogram({0,0,0,1}))-0.8112781245)<.001);
    CHECK(abs(entropy({1,3,3,4,5,8})-2.37165)<.001);
    CHECK(abs(entropy({0,0,0,0,0,0,10}))<.001);
    CHECK(abs(entropy({1,1,1,1,1,1,1,1,1,1})-3.32193)<.001);
}