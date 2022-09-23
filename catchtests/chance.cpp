#include<catch.h>
#include<cell_world.h>
#include <iostream>
using namespace cell_world;
using namespace std;

TEST_CASE("Probabilities Pick index")
{
    CHECK(Chance::pick(vector< unsigned int>{10, 10, 10, 10},0)==0);
    CHECK(Chance::pick(vector< unsigned int>{10, 10, 10, 10},10)==1);
    CHECK(Chance::pick(vector< unsigned int>{10, 10, 10, 10},20)==2);
    CHECK(Chance::pick(vector< unsigned int>{10, 10, 10, 10},30)==3);
}

TEST_CASE("dice") {
    for ( unsigned int i = 0;i<10000;i++)
        CHECK(Chance::dice(100)<=100);
    for ( unsigned int i = 0;i<10000;i++)
        CHECK(Chance::dice(-100,100)>=-100);
    for ( unsigned int i = 0;i<10000;i++)
        CHECK(Chance::dice(-100,100)<=100);
    for ( unsigned int i = 0;i<10000;i++)
        CHECK(Chance::dice_float(100)<=100);
    for ( unsigned int i = 0;i<10000;i++)
        CHECK(Chance::dice_float(-100,100)>=-100);
    for ( unsigned int i = 0;i<10000;i++)
        CHECK(Chance::dice_float(-100,100)<=100);
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
    vector<float>p {.1,.2};
    CHECK(Chance::pick_best(1, p)==1);
    CHECK(Chance::pick_best(0, p,0)==0);
    CHECK(Chance::pick_best(0, p,1)==1);
    CHECK(Chance::pick_best(0, p,2)==0);
    CHECK_THROWS(Chance::coin_toss(1.2));
    CHECK_THROWS(Chance::pick_best(1.2, {.1,.2}));
}

TEST_CASE("get chances") {
//    CHECK(Chance::get_chances({-100.0,100})[0]==0);
//    CHECK(Chance::get_chances({-100.0,100})[1]==CELL_WORLD_CHANCE_MAX);
}

TEST_CASE("invert chances") {
    CHECK(Chance::invert_chances({10,90})[0]==90);
    CHECK(Chance::invert_chances({10,90})[1]==10);
    CHECK(Chance::invert_chances({10,20,90})[0]==90);
    CHECK(Chance::invert_chances({10,20,90})[1]==80);
    CHECK(Chance::invert_chances({10,20,90})[2]==10);
}

TEST_CASE("combine chances") {
    CHECK(Chance::combine_chances({20,20,40},{8,16,20})[0] * 2 == Chance::combine_chances({20,20,40},{8,16,20})[1]);
    CHECK(Chance::combine_chances({20,10,40},{8,16,20})[0] == Chance::combine_chances({20,10,40},{8,16,20})[1]);
    auto a = Chance::get_chances({80, 30, 2}, -100,100);
    auto b = Chance::invert_chances({14,5,5});
    auto c = Chance::combine_chances(a,b);
    CHECK(a[0] == 900);
    CHECK(a[1] == 650);
    CHECK(a[2] == 510);
    CHECK(b[0] == 5);
    CHECK(b[1] == 14);
    CHECK(b[2] == 14);
    CHECK(c[0] == 494);
    CHECK(c[1] == 1000);
    CHECK(c[2] == 784);
    CHECK(Chance::pick(c,487) == 0);
    CHECK(Chance::pick(c,495) == 1);
    CHECK(Chance::pick(c,1495) == 2);
}

TEST_CASE("pick best") {
    vector<float>p{-99.9319,-100,-100};
    CHECK(Chance::pick_best(1,p)==0);
}

TEST_CASE("Experiment::set_name") {
    Experiment experiment;
    experiment.start_time = json_cpp::Json_date::now();
    experiment.occlusions = "10_05";
    experiment.subject_name = "SUBJECT";
    experiment.set_name("PREFIX","SUFFIX");
//    cout << experiment << endl;
//    cout << experiment.start_time << endl;
}

