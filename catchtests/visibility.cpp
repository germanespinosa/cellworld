#include<catch.h>
#include<cell_world.h>

using namespace cell_world;
using namespace std;

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
TEST_CASE("Visibility")
{
    World w("test");
    Cell c0(Circle, {0,1},{0,1},false);
    Cell c1(Circle, {1,1},{1,1},false);
    Cell c2(Circle, {2,1},{2,1},false);
    Cell c3(Circle, {3,1},{3,1},false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    auto cg = w.create_cell_group();
    Graph vi = Visibility::create_graph(cg);
    CHECK(vi.size() == 4);
    CHECK(vi[c0].size() == 4);
    CHECK(vi[c1].size() == 4);
    CHECK(vi[c2].size() == 4);
    CHECK(vi[c3].size() == 4);
}
TEST_CASE("Cone")
{
    CHECK(Visibility_cone::angle_difference(1,2) == 1);
    CHECK(Visibility_cone::angle_difference(2,1) == 1);
    CHECK(Visibility_cone::angle_difference(1,4) == 3);
    CHECK(Visibility_cone::angle_difference(4,1) == 3);
    CHECK(SIMILAR(Visibility_cone::angle_difference(1,5), 2.2831853072, .001));
    CHECK(SIMILAR(Visibility_cone::angle_difference(5,1), 2.2831853072, .001));
    CHECK(Visibility_cone::normalize(3*M_PI)==M_PI);
    CHECK(Visibility_cone::normalize(-M_PI)==M_PI);
    CHECK(Visibility_cone::normalize_degrees(360)==0);
    CHECK(Visibility_cone::normalize_degrees(0)==0);
    CHECK(Visibility_cone::normalize_degrees(181)==-179);
    CHECK(Visibility_cone::to_radians(90)==M_PI/2);
    CHECK(Visibility_cone::to_radians(-90)==3*M_PI/2);
    CHECK(Visibility_cone::to_degrees(M_PI/2)==90);
    CHECK(Visibility_cone::to_degrees(3*M_PI/2)==-90);
}