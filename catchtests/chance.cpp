#include<catch.h>
#include<cell_world.h>
#include <iostream>
using namespace cell_world;
using namespace std;

TEST_CASE("Probabilities Pick index")
{
    CHECK(Chance::pick((vector<uint32_t>){10, 10, 10, 10},0)==0);
    CHECK(Chance::pick((vector<uint32_t>){10, 10, 10, 10},10)==1);
    CHECK(Chance::pick((vector<uint32_t>){10, 10, 10, 10},20)==2);
    CHECK(Chance::pick((vector<uint32_t>){10, 10, 10, 10},30)==3);
}

TEST_CASE("dice") {
    for (uint32_t i = 0;i<10000;i++)
        CHECK(Chance::dice(100)<=100);
    for (uint32_t i = 0;i<10000;i++)
        CHECK(Chance::dice(-100,100)>=-100);
    for (uint32_t i = 0;i<10000;i++)
        CHECK(Chance::dice(-100,100)<=100);
    for (uint32_t i = 0;i<10000;i++)
        CHECK(Chance::dice_double(100)<=100);
    for (uint32_t i = 0;i<10000;i++)
        CHECK(Chance::dice_double(-100,100)>=-100);
    for (uint32_t i = 0;i<10000;i++)
        CHECK(Chance::dice_double(-100,100)<=100);
}

TEST_CASE("index") {
    auto i = new_index({.1,.2,.3,.05}, false);
    CHECK( i[0] == 2);
    CHECK( i[1] == 1);
    CHECK( i[2] == 0);
    CHECK( i[3] == 3);
    i = new_index({.1,.2,.3,.05}, true);
    CHECK( i[0] == 3);
    CHECK( i[1] == 0);
    CHECK( i[2] == 1);
    CHECK( i[3] == 2);
}

TEST_CASE("pick occurrence") {
    CHECK(Chance::pick_random_occurrence({.1,.2,.3,.1},.1,0)==0);
    CHECK(Chance::pick_random_occurrence({.1,.2,.3,.1},.1,1)==3);
}

TEST_CASE("pick by chance") {
    CHECK(Chance::pick_by_chance({.1,.2,.3,.1},{100,10,10,10},0)==2);
    CHECK(Chance::pick_by_chance({.1,.2,.3,.1},{100,10,10,10},100)==1);
    CHECK(Chance::pick_by_chance({.1,.2,.3,.1},{100,10,10,10},110)==0);
    CHECK(Chance::pick_by_chance({.1,.2,.3,.1},{100,10,10,10},111)==3);
}

TEST_CASE("coin toss") {
    CHECK(Chance::coin_toss(1));
    CHECK(!Chance::coin_toss(0));
    CHECK(Chance::coin_toss(.5,1));
    CHECK(Chance::coin_toss(.5,CELL_WORLD_CHANCE_MAX/2-1));
    CHECK(!Chance::coin_toss(.5,CELL_WORLD_CHANCE_MAX/2+1));
    CHECK(!Chance::coin_toss(.5,CELL_WORLD_CHANCE_MAX-1));
    CHECK(Chance::pick_best(1, {.1,.2})==1);
    CHECK(Chance::pick_best(0, {.1,.2},0)==0);
    CHECK(Chance::pick_best(0, {.1,.2},1)==1);
    CHECK(Chance::pick_best(0, {.1,.2},2)==0);
    CHECK_THROWS(Chance::coin_toss(1.2));
    CHECK_THROWS(Chance::pick_best(1.2, {.1,.2}));
}
