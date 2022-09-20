#include <catch.h>
#include <cell_world.h>

using namespace std;
using namespace json_cpp;
using namespace cell_world;

TEST_CASE("key tests"){
    auto wr = Resources::from("cell_group");
    CHECK(wr.url() == "https://raw.githubusercontent.com/germanespinosa/cellworld_data/master/cell_group/");
    wr.key("hexagonal");
    CHECK(wr.url() == "https://raw.githubusercontent.com/germanespinosa/cellworld_data/master/cell_group/hexagonal");
    wr.key("00_00");
    CHECK(wr.url() == "https://raw.githubusercontent.com/germanespinosa/cellworld_data/master/cell_group/hexagonal.00_00");
}

TEST_CASE("get_world_configuration"){
    CHECK_NOTHROW(Json_create<World_configuration>(Resources::from("world_configuration").key("hexagonal").get()));
}

//TEST_CASE("get_cell_group"){
//    CHECK_NOTHROW(Json_create<Cell_group>(Resources::from("cell_group").key("hexagonal").key("10_05").key("occlusions").get()));
//}

TEST_CASE("CACHE FOLDER"){
    CHECK(!Resources::cache_folder().empty());
}

TEST_CASE("World_implementation transformation"){
    auto wc =  Json_create<World_configuration>(Resources::from("world_configuration").key("hexagonal").get());
    auto wi =  Json_create<World_implementation>(Resources::from("world_implementation").key("hexagonal").key("canonical").get());

    Space new_space;
    new_space.center = {250,250};
    new_space.transformation.size = (double)500;
    new_space.transformation.rotation = wi.space.transformation.rotation;
    new_space.shape.sides = 6;
    //cout << wi.cell_locations.get_x() << "," << wi.cell_locations.get_y() << endl << endl;
    wi.transform(new_space);
    //cout << wi.cell_locations.get_x() << "," << wi.cell_locations.get_y() << endl;
}

#include <stdio.h>

TEST_CASE("world_implementation"){
//    for (int i=0; i<100;i++) {
//        cout << i << endl;
//        cout << Resources::from("world_implementation").key("hexagonal").key("vr").get_resource<World_implementation>() << endl;
//    }

}
