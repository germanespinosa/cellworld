#include<catch.h>
#include<cellworld.h>
#include <iostream>
using namespace cell_world;
using namespace std;

TEST_CASE("Probabilities Pick")
{
    Probabilities p((vector<uint32_t>){10,10,10,10});
    CHECK(p.pick({1,2,3,4},0)==3);
    CHECK(p.pick({1,2,3,4},10)==2);
    CHECK(p.pick({1,2,3,4},20)==1);
    CHECK(p.pick({1,2,3,4},30)==0);
    CHECK(p.pick({1,2,2,2},0)==1);
    CHECK(p.pick({1,2,2,2},1)==2);
    CHECK(p.pick({1,2,2,2},2)==3);
}

TEST_CASE("Probabilities Compute")
{
    Probabilities p((vector<double>){.1,.1,.1,.1,.1,.5});
    CHECK(p.compute({10,20,30,40,50,20})==25);
}