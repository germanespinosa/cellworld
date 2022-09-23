#include<catch.h>
#include<cell_world.h>
#include <cell_world/geometry.h>

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

    CHECK(abs(weights_entropy(histogram({0,0,0,1}))-.81128)<.001);
    CHECK(abs(weights_entropy(vector<unsigned int>{1,3,3,4,5,8})-0.9175)<.001);
    CHECK(abs(weights_entropy(vector<unsigned int>{0,0,0,0,0,0,10}))<.001);
    CHECK(abs(weights_entropy(vector<unsigned int>{1,1,1,1,1,1,1,1,1,1})-1.0)<.001);
}
TEST_CASE("Visibility")
{
    World w;
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
    CHECK(angle_difference(1, 2) == 1);
    CHECK(direction(1, 2) == -1);
    CHECK(angle_difference(2, 1) == 1);
    CHECK(direction(2, 1) == 1);
    CHECK(angle_difference(1, 4) == 3);
    CHECK(angle_difference(4, 1) == 3);
    CHECK(SIMILAR(angle_difference(1, 5), 2.2831853072, .001));
    CHECK(SIMILAR(angle_difference(5, 1), 2.2831853072, .001));
    CHECK(SIMILAR(normalize(3 * M_PI), M_PI, .001));
    CHECK(SIMILAR(normalize(-M_PI), M_PI, .001));
    CHECK(normalize_degrees(360) == 0);
    CHECK(normalize_degrees(0) == 0);
    CHECK(normalize_degrees(181) == -179);
    CHECK(SIMILAR(normalize(to_radians(90)), M_PI / 2, .001));
    CHECK(SIMILAR(normalize(to_radians(-90)), 3 * M_PI / 2, .001));
    CHECK(to_degrees(M_PI / 2) == 90);
    CHECK(normalize_degrees(to_degrees(3 * M_PI / 2)) == -90);
}


TEST_CASE("is_occluding")
{
    Polygon p (Location (0,0), 4, 1 , 45);

    CHECK(p.is_between({-3,0},{3,0}));

    CHECK(p.is_between({.7,-3},{.7,3}));
    CHECK(p.is_between({-.7,-3},{-.7,3}));
    CHECK(!p.is_between({.8,-3},{.8,3}));
    CHECK(!p.is_between({-.8,-3},{-.8,3}));
    CHECK(!p.is_between({-3,.8},{3,.8}));
    CHECK(!p.is_between({-3,-.8},{3,-.8}));
    CHECK(p.is_between({-3,.7},{3,.7}));
    CHECK(p.is_between({-3,-.7},{3,-.7}));
    CHECK(!p.is_between({2,0},{3,0}));
    CHECK(!p.is_between({-2,0},{-3,0}));
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
    Polygon_list occlusions;
    Location source;
    json_cpp::Json_vector<Tvr> records;
    Json_object_members(
            Add_member(source);
            Add_member(occlusions);
            Add_member(records);
    )
};

Tv test_visibility(const Polygon &p) {
    Tv tv;
    tv.occlusions.push_back(p);
    tv.source = p.center.move(0,p.radius * 2);
    for (double i = 0; i < 360; i++){
        double theta = to_radians(i );
        Tvr tvr;
        tvr.destination = tv.source.move(theta, p.radius * 4);
        tvr.visible = !p.is_between( tv.source, tvr.destination);
        tv.records.push_back(tvr);
    }
    return tv;
}

TEST_CASE("test_visibility"){
    test_visibility(Polygon({0,0},3,1,60)).save("triangle.json");
    test_visibility(Polygon({0,0},4,1,35)).save("square.json");
    test_visibility(Polygon({0,0},6,1,0)).save("hexagon.json");
}


Tv test_location_visibility(const Location_visibility &lv, Location src, double radius) {
    Tv tv;
    tv.occlusions = lv.occlusions;
    tv.source = src;
    for (double i = 0; i < 360; i++){
        double theta = to_radians(i );
        Tvr tvr;
        tvr.destination = tv.source.move(theta, radius);
        tvr.visible = lv.is_visible(tv.source, tvr.destination);
        tv.records.push_back(tvr);
    }
    return tv;
}

TEST_CASE("test_location_visibility"){
    Cell c0;
    "{\"id\":1,\"coordinates\":{\"x\":0,\"y\":0},\"location\":{\"x\":0,\"y\":0},\"occluded\":1}" >> c0;
    Cell c1;
    "{\"id\":2,\"coordinates\":{\"x\":1,\"y\":1},\"location\":{\"x\":0,\"y\":4},\"occluded\":1}" >> c1;
    Cell_group cg;
    cg.add(c0);
    cg.add(c1);
    Location_visibility lv(cg, Shape(3), Transformation(1,0));
    test_location_visibility(lv, {-3,2}, 4).save("multiple.json");
}