#include <json_cpp.h>
#include <boost/python.hpp>
#include <boost/python/list.hpp>
#include <boost/python/extract.hpp>
#include <cell_world/core.h>
#include <cell_world/coordinates.h>
#include <cell_world/location.h>
#include <cell_world/transformation.h>
#include <cell_world/cell.h>
#include <cell_world/shape.h>
#include <cell_world/world.h>
#include <cell_world/geometry.h>
#include <cell_world/cell_group.h>
#include <cell_world/graph.h>
#include <cell_world/map.h>
#include <cell_world/timer.h>

using namespace boost::python;
using namespace json_cpp;
using namespace std;

using namespace cell_world;
BOOST_PYTHON_MODULE(core)
{
    class_<Json_base, boost::noncopyable>("Json_object")
            .def("load", &Json_object::load)
            .def("save", &Json_object::save)
            .def("__str__", &Json_object::to_json)
            .def("to_json", &Json_object::to_json)
            .def("from_json", &Json_object::from_json)
            ;

    class_<Json_int_vector>("Json_int_vector")
            .def(init<size_t>())
            .def(init<size_t, int>())
            .def("load", &Json_vector<Location>::load)
            .def("save", &Json_vector<Location>::save)
            .def("__str__", &Json_vector<Location>::to_json)
            .def("to_json", &Json_vector<Location>::to_json)
            .def("from_json", &Json_vector<Location>::from_json)
            .def("__getitem__", &Json_vector<Location>::get_item_at)
            .def("__setitem__", &Json_vector<Location>::set_item_at)
            ;

    class_<Json_bool_vector>("Json_bool_vector")
            .def(init<size_t>())
            .def(init<size_t, bool>())
            .def("load", &Json_vector<Location>::load)
            .def("save", &Json_vector<Location>::save)
            .def("__str__", &Json_vector<Location>::to_json)
            .def("to_json", &Json_vector<Location>::to_json)
            .def("from_json", &Json_vector<Location>::from_json)
            .def("__getitem__", &Json_vector<Location>::get_item_at)
            .def("__setitem__", &Json_vector<Location>::set_item_at)
            ;

    class_<Json_unsigned_int_vector>("Json_unsigned_int_vector")
            .def(init<size_t>())
            .def(init<size_t, unsigned int>())
            .def("load", &Json_vector<Location>::load)
            .def("save", &Json_vector<Location>::save)
            .def("__str__", &Json_vector<Location>::to_json)
            .def("to_json", &Json_vector<Location>::to_json)
            .def("from_json", &Json_vector<Location>::from_json)
            .def("__getitem__", &Json_vector<Location>::get_item_at)
            .def("__setitem__", &Json_vector<Location>::set_item_at)
            ;

    class_<Json_float_vector>("Json_float_vector")
            .def(init<size_t>())
            .def(init<size_t, float>())
            .def("load", &Json_vector<Location>::load)
            .def("save", &Json_vector<Location>::save)
            .def("__str__", &Json_vector<Location>::to_json)
            .def("to_json", &Json_vector<Location>::to_json)
            .def("from_json", &Json_vector<Location>::from_json)
            .def("__getitem__", &Json_vector<Location>::get_item_at)
            .def("__setitem__", &Json_vector<Location>::set_item_at)
            ;

    class_<Json_vector<Location>, boost::noncopyable>("Json_location_vector")
            .def(init<size_t>())
            .def(init<size_t, Location>())
            .def("load", &Json_vector<Location>::load)
            .def("save", &Json_vector<Location>::save)
            .def("__str__", &Json_vector<Location>::to_json)
            .def("to_json", &Json_vector<Location>::to_json)
            .def("from_json", &Json_vector<Location>::from_json)
            .def("__getitem__", &Json_vector<Location>::get_item_at)
            .def("__setitem__", &Json_vector<Location>::set_item_at)
            ;


    class_<Json_vector<Coordinates>, bases<Json_base>>("Coordinates_list")
            .def(init<size_t>())
            .def(init<size_t, Coordinates>())
            .def("load", &Json_vector<Location>::load)
            .def("save", &Json_vector<Location>::save)
            .def("__str__", &Json_vector<Location>::to_json)
            .def("to_json", &Json_vector<Location>::to_json)
            .def("from_json", &Json_vector<Location>::from_json)
            .def("__getitem__", &Json_vector<Location>::get_item_at)
            .def("__setitem__", &Json_vector<Location>::set_item_at)
            ;

    class_<Coordinates, bases<Json_base>>("Coordinates")
            .def(init<int, int>())
            .def_readwrite("x", &Coordinates::x)
            .def_readwrite("y", &Coordinates::y)
            .def("manhattan", &Coordinates::manhattan)
            .def("is_origin", &Coordinates::is_origin)
            .def("rotation", &Coordinates::rotation)
            .def(-self)
            .def(self + Coordinates())
            .def(self - Coordinates())
            .def(self == Coordinates())
            .def(self != Coordinates())
            .def(self += Coordinates())
            ;

    float (Location::*location_atan_0)() const = &Location::atan;
    float (Location::*location_atan_1)(const Location &) const = &Location::atan;

    float (Location::*location_dist_0)() const = &Location::dist;
    float (Location::*location_dist_1)(const Location &) const = &Location::dist;
    float (Location::*location_dist_2)(const Location &, const Location &) const = &Location::dist;

    Transformation (Location::*location_transformation_0)() const = &Location::transformation;
    Transformation (Location::*location_transformation_1)(const Location &) const = &Location::transformation;

    class_<Location, bases<Json_base>>("Location")
            .def(init<float, float>())
            .def_readwrite("x", &Location::x)
            .def_readwrite("y", &Location::y)
            .def(-self)
            .def(self + Location())
            .def(self - Location())
            .def(self == Location())
            .def(self != Location())
            .def(self += Location())
            .def(self * Location())
            .def(self * float())
            .def(self / float())
            .def("mod",  &Location::mod)
            .def("move",  &Location::move)
            .def("transform",  &Location::transform)
            .def("transformation",  location_transformation_0)
            .def("transformation",  location_transformation_1)
            .def("atan", location_atan_0)
            .def("atan", location_atan_1)
            .def("dist", location_dist_0)
            .def("dist", location_dist_1)
            .def("dist", location_dist_2)
            ;

    class_<Location_list, bases<Json_vector<Location>>>("Location_list")
            .def("get_x", &Location_list::get_x)
            .def("get_y", &Location_list::get_y)
            ;

    class_<Transformation, bases<Json_base>>("Transformation")
            .def(init<float, float>())
            .def_readwrite("size", &Transformation::size)
            .def_readwrite("rotation", &Transformation::rotation)
            .def("theta",&Transformation::theta)
            ;

    class_<Cell, bases<Json_base>>("Cell")
            .def(init<>())
            .def(init<const Coordinates &>())
            .def(init<Coordinates, Location, bool>())
            .def(init<const Cell &>())
            .def_readwrite("id", &Cell::id)
            .def_readwrite("coordinates", &Cell::coordinates)
            .def_readwrite("location", &Cell::location)
            .def_readwrite("occluded", &Cell::occluded)
            .def(self == Cell())
            .def(self != Cell())
            ;


    class_<Json_vector<Cell>, bases<Json_base>>("Cell_list")
            .def(init<size_t>())
            .def(init<size_t, Cell>())
            .def("__getitem__", &Cell_list::get_item_at)
            .def("__setitem__", &Cell_list::set_item_at)
            ;

    class_<Shape, bases<Json_base>>("Shape")
            .def(init<>())
            .def(init<int>())
            .def_readwrite("sides", &Shape::sides)
            ;

    bool (Polygon::*polygon_contains_0)(const Location &) const = &Polygon::contains;
    bool (Polygon::*polygon_contains_1)(const Polygon &) const = &Polygon::contains;

    Polygon (Polygon::*polygon_move_0)(const Location &) const = &Polygon::move;
    Polygon (Polygon::*polygon_move_1)(float, float) const = &Polygon::move;

    class_<Polygon, bases<Json_base>>("Polygon")
            .def(init<>())
            .def(init<const Polygon &>())
            .def(init<const Location, unsigned int, float, float >())
            .def(init<const Location &,const Shape &, const Transformation &>())
            .def_readwrite("vertices", &Polygon::vertices)
            .def_readwrite("center", &Polygon::center)
            .def_readwrite("radius", &Polygon::radius)
            .def("contains", polygon_contains_0)
            .def("contains", polygon_contains_1)
            .def("overlaps", &Polygon::overlaps)
            .def("contains", polygon_move_0)
            .def("contains", polygon_move_1)
            .def(self += Location())
            ;

    class_<Polygon_list, bases<Json_base>>("Polygon_list")
            .def(init<>())
            .def(init<const Location_list &, const Shape &, const Transformation &>())
            .def("__getitem__", &Polygon_list::get_item_at)
            .def("__setitem__", &Polygon_list::set_item_at)
            .def("contains", &Polygon_list::contains)
            ;

    class_<Space, bases<Json_base>>("Space")
            .def(init<>())
            .def(init<const Location &, const Shape &, const Transformation &>())
            .def_readwrite("center", &Space::center)
            .def_readwrite("shape", &Space::shape)
            .def_readwrite("transformation", &Space::transformation)
            .def("transform", &Space::transform)
            .def("scale", &Space::scale)
            ;

    class_<World_info, bases<Json_base>>("World_info")
            .def(init<>())
            .def_readwrite("world_configuration", &World_info::world_configuration)
            .def_readwrite("world_implementation", &World_info::world_implementation)
            .def_readwrite("occlusions", &World_info::occlusions)
            ;

    class_<World_configuration, bases<Json_base>>("World_configuration")
            .def(init<>())
            .def(init<const World_configuration &>())
            .def(init<const Shape &, const Coordinates_list &, const Connection_pattern &>())
            .def_readwrite("cell_shape", &World_configuration::cell_shape)
            .def_readwrite("cell_coordinates", &World_configuration::cell_coordinates)
            .def_readwrite("connection_pattern", &World_configuration::connection_pattern)
            .def("get_from_parameters_name", &World_configuration::get_from_parameters_name).staticmethod("get_from_parameters_name")
            ;

    class_<World_implementation, bases<Json_base>>("World_implementation")
            .def(init<>())
            .def(init<const World_implementation &>())
            .def(init<const Location_list &, const Space &, const Transformation &>())
            .def_readwrite("cell_locations", &World_implementation::cell_locations)
            .def_readwrite("space", &World_implementation::space)
            .def_readwrite("cell_transformation", &World_implementation::cell_transformation)
            .def("transform",&World_implementation::transform)
            .def("scale",&World_implementation::scale)
            .def("get_from_parameters_name", &World_implementation::get_from_parameters_name).staticmethod("get_from_parameters_name")
            ;

    class_<World_statistics, bases<Json_base>>("World_statistics")
            .def(init<>())
            .def_readwrite("spatial_entropy", &World_statistics::spatial_entropy)
            .def_readwrite("spatial_espinometry", &World_statistics::spatial_espinometry)
            .def_readwrite("spatial_connections", &World_statistics::spatial_connections)
            .def_readwrite("spatial_connections_derivative", &World_statistics::spatial_connections_derivative)
            .def_readwrite("spatial_centrality", &World_statistics::spatial_centrality)
            .def_readwrite("spatial_centrality_derivative", &World_statistics::spatial_centrality_derivative)
            .def_readwrite("visual_entropy", &World_statistics::visual_entropy)
            .def_readwrite("visual_espinometry", &World_statistics::visual_espinometry)
            .def_readwrite("visual_connections", &World_statistics::visual_connections)
            .def_readwrite("visual_connections_derivative", &World_statistics::visual_connections_derivative)
            .def_readwrite("visual_centrality", &World_statistics::visual_centrality)
            .def_readwrite("visual_centrality_derivative", &World_statistics::visual_centrality_derivative)
            .def("get_from_parameters_name", &World_statistics::get_from_parameters_name)
            ;

    def("angle_difference", angle_difference);
    def("direction", direction);
    def("to_radians", to_radians);
    def("to_degrees", to_degrees);
    def("normalize", normalize);
    def("normalize_degrees", normalize_degrees);
    def("segments_intersect", segments_intersect);
    def("angle_between", angle_between);


    class_<Cell_group_builder, bases<Json_base>>("Cell_group_builder")
            .def(init<>())
            .def("__getitem__", &Cell_group_builder::get_item_at)
            .def("__setitem__", &Cell_group_builder::set_item_at)
            .def("get_from_parameters_name", +[](const string &configuration, const string &occlusions, const string &name){return Cell_group_builder::get_from_parameters_name(configuration, occlusions, name);})
            .def("get_from_parameters_name", +[](const string &configuration, const string &name){return Cell_group_builder::get_from_parameters_name(configuration, name);})
            ;



    bool (Cell_group::*cell_group_contains_0)(unsigned int) const = &Cell_group::contains;
    bool (Cell_group::*cell_group_contains_1)(const Cell &) const = &Cell_group::contains;

    int (Cell_group::*Cell_group_find_0)(unsigned int) const = &Cell_group::find;
    int (Cell_group::*Cell_group_find_1)(const Cell &) const = &Cell_group::find;
    int (Cell_group::*Cell_group_find_2)(const Location &) const = &Cell_group::find;
    int (Cell_group::*Cell_group_find_3)(const Coordinates &) const = &Cell_group::find;

    float (Cell_group::*Cell_group_distance_0)(unsigned int, unsigned int) const = &Cell_group::distance;
    static float (*Cell_group_distance_1)(const Cell&, const Cell&) = &Cell_group::distance;


    class_<Cell_group, bases<Json_base>>("Cell_group")
            .def(init<>())
            .def(init<const cell_world::Cell_group&>())
            .def("__getitem__", +[](const Cell_group& cg, size_t i){return cg[i];})
            .def("contains", cell_group_contains_0)
            .def("contains", cell_group_contains_1)
            .def("clear", &Cell_group::clear)
            .def("find", Cell_group_find_0)
            .def("find", Cell_group_find_1)
            .def("find", Cell_group_find_2)
            .def("find", Cell_group_find_3)
            .def("distance", Cell_group_distance_0)
            .def("distance", Cell_group_distance_1)
            .def("get_distances", &Cell_group::get_distances)
            .def("random_shuffle", &Cell_group::random_shuffle)
            .def("occluded_cells", &Cell_group::occluded_cells)
            .def("free_cells", &Cell_group::free_cells)
            .def("get_builder", &Cell_group::get_builder)
            .def("random_cell", &Cell_group::get_builder)
            .def(self + Cell_group())
            .def(self - Cell_group())
            .def(self - Cell())
            .def(self + Cell())
            .def(self == Cell_group())
            .def(self != Cell_group())
            ;

    bool (Graph::*graph_add_0)(const Cell&) = &Graph::add;
    bool (Graph::*graph_add_1)(const Cell_group &) = &Graph::add;

    void (Graph::*graph_connect_0)(const Cell&, const Cell_group &) = &Graph::connect;
    void (Graph::*graph_connect_1)(Graph &) = &Graph::connect;

    class_<Graph, bases<Json_base>>("Graph")
            .def(init<>())
            .def(init<const cell_world::Cell_group&>())
            .def(init<const cell_world::Graph&>())
            .def("add", graph_add_0)
            .def("add", graph_add_1)
            .def("connect", graph_connect_0)
            .def("connect", graph_connect_1)
            .def("get_connectors", &Graph::get_connectors)
            .def("get_entropy", &Graph::get_entropy)
            .def("invert", &Graph::invert)
            .def("get_shortest_path", &Graph::get_shortest_path)
            .def("get_centrality", &Graph::get_centrality)
            .def("is_connected", &Graph::is_connected)
            .def(!self)
            .def(self == Graph())
            .def("__getitem__", +[](const Graph& cg, size_t i){return cg[i];})
            .def("__getitem__", +[](const Graph& cg, const Cell &c){return cg[c.id];})
            ;

    bool (World::*world_add_0)(Cell) = &World::add;
    bool (World::*world_add_1)(Coordinates) = &World::add;

    Cell_group (World::*world_create_cell_group_0)() const = &World::create_cell_group;
    Cell_group (World::*world_create_cell_group_1)(const Cell_group_builder &) const = &World::create_cell_group;
    Cell_group (World::*world_create_cell_group_2)(const std::string &) const = &World::create_cell_group;

    Graph (World::*world_create_graph_0)() const = &World::create_graph;
    Graph (World::*world_create_graph_1)(const Graph_builder &) const = &World::create_graph;

    World (*world_get_from_parameters_name_0)(const std::string &, const std::string &, const std::string &) = &World::get_from_parameters_name;
    World (*world_get_from_parameters_name_1)(const std::string &, const std::string &) = &World::get_from_parameters_name;
    World (*world_get_from_world_info_0)(const World_info &) = &World::get_from_world_info;

    class_<World, bases<Json_base>>("World")
            .def(init<>())
            .def(init<const World_configuration &>())
            .def(init<const World_configuration &, const World_implementation &>())
            .def(init<const World_configuration &, const World_implementation &, const Cell_group_builder &>())
            .def_readwrite("connection_pattern", &World::connection_pattern)
            .def_readwrite("cells", &World::cells)
            .def_readwrite("cell_shape", &World::cell_shape)
            .def_readwrite("cell_transformation", &World::cell_transformation)
            .def("add", world_add_0)
            .def("add", world_add_1)
            .def("create_cell_group", world_create_cell_group_0)
            .def("create_cell_group", world_create_cell_group_1)
            .def("create_cell_group", world_create_cell_group_2)
            .def("create_graph", world_create_graph_0)
            .def("create_graph", world_create_graph_1)
            .def("create_paths", &World::create_paths)
            .def("set_occlusions", &World::set_occlusions)
            .def("size", &World::size)
            .def("create_location_visibility", &World::create_location_visibility)
            .def("get_configuration", &World::get_configuration)
            .def("get_implementation", &World::get_implementation)
            .def("get_statistics", &World::get_statistics)
            .def("get_from_parameters_name", world_get_from_parameters_name_0)
            .def("get_from_parameters_name", world_get_from_parameters_name_1)
            .def("get_from_world_info", world_get_from_world_info_0)
            ;

    class_<Map>("Map", init<const cell_world::Cell_group&>())
            .def("__getitem__", +[](const Map& m, const Coordinates &c){return m[c];})
            .def("__getitem__", +[](const Map& m, int x, int y){return m[Coordinates(x,y)];})
            .def("find", &Map::find)
            .def_readwrite("cells", &Map::cells)
            ;

    class_<Path_builder>("Path_builder")
            .def(init<>())
            .def_readwrite("moves", &Path_builder::moves)
            .def_readwrite("steps", &Path_builder::steps)
            .def("get_from_parameters_name", &Path_builder::get_from_parameters_name)
            ;

    class_<Paths>("Paths")
            .def(init<>())
            .def(init<const Graph&, Move_list >())
            .def(init<const Graph&, const Path_builder &>())
            .def("get_move", &Paths::get_move)
            .def("get_steps", &Paths::get_steps)
            .def("get_steps", &Paths::get_steps)
            .def("get_path", &Paths::get_path)
            .def("get_moves", &Paths::get_moves)
            .def("set_move", &Paths::set_move)
            .def("__getitem__", +[](const Map& m, const Coordinates &c){return m[c];})
            .def("__getitem__", +[](const Map& m, int x, int y){return m[Coordinates(x,y)];})
            .def("find", &Map::find)
            ;

    class_<Timer>("Timer")
            .def(init<>())
            .def(init<float>())
            .def("reset", &Timer::reset)
            .def("to_seconds", &Timer::to_seconds)
            .def("time_out", &Timer::time_out)
            .def("wait", &Timer::wait)
            ;

    class_<Coordinates_visibility>("Coordinates_visibility")
            .def("create_graph", &Coordinates_visibility::create_graph)
            .def("invert", &Coordinates_visibility::invert)
            ;

    class_<Coordinates_visibility_cone>("Coordinates_visibility_cone", init<const Graph &, float>())
            .def("visible_cells", &Coordinates_visibility_cone::visible_cells)
            .def("is_visible", &Coordinates_visibility_cone::is_visible)
            ;

    class_<Location_visibility>("Location_visibility", init<const Shape &, const Transformation &>())
            .def(init<const Cell_group &, const Shape &, const Transformation &>())
            .def("is_visible", +[](const Location_visibility& lv, const Location &src, const Location &dst){ return lv.is_visible(src, dst);})
            .def("is_visible", +[](const Location_visibility& lv, const Location &src, float src_theta, float src_cone, const Location &dst){ return lv.is_visible(src, src_theta, src_cone, dst);})
            .def("is_visible_multi", &Location_visibility::is_visible_multi)
            ;
}

