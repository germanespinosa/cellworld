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
    Cell c0({0,1},{0,1},false);
    Cell c1({1,1},{1,1},false);
    Cell c2({2,1},{2,1},false);
    Cell c3({3,1},{3,1},false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    auto cg = w.create_cell_group();
    Shape desc(6);
    Transformation t(.5001,0);
    Graph vi = Coordinates_visibility::create_graph(cg, desc, t);
    CHECK(vi.size() == 4);
    CHECK(vi[c0].size() == 4);
    CHECK(vi[c1].size() == 4);
    CHECK(vi[c2].size() == 4);
    CHECK(vi[c3].size() == 4);
}


TEST_CASE("Cone")
{
    CHECK(Visibility::angle_difference(1, 2) == 1);
    CHECK(Visibility::direction(1, 2) == -1);
    CHECK(Visibility::angle_difference(2, 1) == 1);
    CHECK(Visibility::direction(2, 1) == 1);
    CHECK(Visibility::angle_difference(1, 4) == 3);
    CHECK(Visibility::angle_difference(4, 1) == 3);
    CHECK(SIMILAR(Visibility::angle_difference(1, 5), 2.2831853072, .001));
    CHECK(SIMILAR(Visibility::angle_difference(5, 1), 2.2831853072, .001));
    CHECK(Visibility::normalize(3 * M_PI) == M_PI);
    CHECK(Visibility::normalize(-M_PI) == M_PI);
    CHECK(Visibility::normalize_degrees(360) == 0);
    CHECK(Visibility::normalize_degrees(0) == 0);
    CHECK(Visibility::normalize_degrees(181) == -179);
    CHECK(Visibility::to_radians(90) == 3 * M_PI / 2);
    CHECK(Visibility::to_radians(-90) == M_PI / 2);
    CHECK(Visibility::to_degrees(M_PI / 2) == 90);
    CHECK(Visibility::to_degrees(3 * M_PI / 2) == -90);
}


TEST_CASE("is_occluding")
{
    Polygon p (Location (0,0), 4, 1 , 45);

    CHECK(Visibility::is_occluding({-3,0},{3,0},p));

    CHECK(Visibility::is_occluding({.7,-3},{.7,3},p));
    CHECK(Visibility::is_occluding({-.7,-3},{-.7,3},p));
    CHECK(!Visibility::is_occluding({.8,-3},{.8,3},p));
    CHECK(!Visibility::is_occluding({-.8,-3},{-.8,3},p));
    CHECK(!Visibility::is_occluding({-3,.8},{3,.8},p));
    CHECK(!Visibility::is_occluding({-3,-.8},{3,-.8},p));
    CHECK(Visibility::is_occluding({-3,.7},{3,.7},p));
    CHECK(Visibility::is_occluding({-3,-.7},{3,-.7},p));
    CHECK(!Visibility::is_occluding({2,0},{3,0},p));
    CHECK(!Visibility::is_occluding({-2,0},{-3,0},p));
}

struct Tvr : json_cpp::Json_object {
    Location destination;
    bool visible;

    Json_object_members(
            Add_member(destination);
            Add_member(visible);
    )
};

struct Tv : json_cpp::Json_object {
    Polygon occlusion;
    Location source;
    json_cpp::Json_vector<Tvr> records;
    Json_object_members(
            Add_member(source);
            Add_member(occlusion);
            Add_member(records);
    )
};

Tv test_visibility(const Polygon &p) {
    Tv tv;
    tv.occlusion = p;
    tv.source = p.center.move(0,p.radius * 2);
    for (double i = 0; i < 360; i++){
        double theta = Visibility::to_radians(i );
        Tvr tvr;
        tvr.destination = tv.source.move(theta, p.radius * 4);
        tvr.visible = !Visibility::is_occluding( tv.source, tvr.destination, p);
        tv.records.push_back(tvr);
    }
    return tv;
}

TEST_CASE("test_visibility"){
    test_visibility(Polygon({0,0},3,1,60)).save("triangle.json");
    test_visibility(Polygon({0,0},4,1,35)).save("square.json");
    test_visibility(Polygon({0,0},6,1,0)).save("hexagon.json");
}