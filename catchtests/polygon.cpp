#include <random>
#include<catch.h>
#include<cell_world.h>
#include<sstream>

using namespace cell_world;
using namespace std;

TEST_CASE("polygon")
{
    Polygon p (Location (0,0), 4, 1 , 45);
    CHECK (p.to_json() == "{\"vertices\":[{\"x\":0.707107,\"y\":0.707107},{\"x\":0.707107,\"y\":-0.707107},{\"x\":-0.707107,\"y\":-0.707107},{\"x\":-0.707107,\"y\":0.707107}],\"center\":{\"x\":0,\"y\":0},\"radius\":1}" );
    CHECK( p.move(0,1).to_json() == "{\"vertices\":[{\"x\":0.707107,\"y\":1.70711},{\"x\":0.707107,\"y\":0.292893},{\"x\":-0.707107,\"y\":0.292893},{\"x\":-0.707107,\"y\":1.70711}],\"center\":{\"x\":0,\"y\":1},\"radius\":1}");
    CHECK (p.move({1,1}).to_json() == "{\"vertices\":[{\"x\":1.70711,\"y\":1.70711},{\"x\":1.70711,\"y\":0.292893},{\"x\":0.292893,\"y\":0.292893},{\"x\":0.292893,\"y\":1.70711}],\"center\":{\"x\":1,\"y\":1},\"radius\":1}");
    CHECK (p.to_json() == "{\"vertices\":[{\"x\":0.707107,\"y\":0.707107},{\"x\":0.707107,\"y\":-0.707107},{\"x\":-0.707107,\"y\":-0.707107},{\"x\":-0.707107,\"y\":0.707107}],\"center\":{\"x\":0,\"y\":0},\"radius\":1}" );
    CHECK ((p += Location{1,1}).to_json() == "{\"vertices\":[{\"x\":1.70711,\"y\":1.70711},{\"x\":1.70711,\"y\":0.292893},{\"x\":0.292893,\"y\":0.292893},{\"x\":0.292893,\"y\":1.70711}],\"center\":{\"x\":1,\"y\":1},\"radius\":1}");
    CHECK (p.to_json() == "{\"vertices\":[{\"x\":1.70711,\"y\":1.70711},{\"x\":1.70711,\"y\":0.292893},{\"x\":0.292893,\"y\":0.292893},{\"x\":0.292893,\"y\":1.70711}],\"center\":{\"x\":1,\"y\":1},\"radius\":1}");
    //cout << Polygon({0,0},6,1,0).vertices.get_x()  << "," << Polygon({0,0},6,1,0).vertices.get_y() << endl;
}

TEST_CASE("polygon_ contains")
{
    auto wc = Resources::from("world_configuration").key("hexagonal").get_resource<World_configuration>();
    auto wi = Resources::from("world_implementation").key("hexagonal").key("mice").get_resource<World_implementation>();
    auto occlusions = Resources::from("cell_group").key("hexagonal").key("10_05").key("occlusions").get_resource<Cell_group_builder>();
    World world(wc, wi, occlusions);
    Polygon habitat_polygon (wi.space.center,wi.space.shape,wi.space.transformation);
    Polygon_list cell_polygons;
    cell_polygons.clear();
    auto occluded_cells = world.create_cell_group().occluded_cells();
    for (auto &cell: occluded_cells) {
        cell_polygons.push_back(Polygon(cell.get().location,world.cell_shape, world.cell_transformation));
    }
    Location_list in_cell;
    Location_list in_habitat;
    Location_list out_habitat;
    for (int i=0; i<10000; i++) {
        auto point = wi.space.center.move(Chance::dice_float(0,6.28), Chance::dice_float(0,1.5));
        if (habitat_polygon.contains(point)) {
            if (cell_polygons.contains(point)) {
                in_cell.push_back(point);
            } else {
                in_habitat.push_back(point);
            }
        } else {
            out_habitat.push_back(point);
        }
    }
    in_cell.save("in_cell.json");
    in_habitat.save("in_habitat.json");
    out_habitat.save("out_habitat.json");
}