#include <map_editor.h>
#include <iostream>
#include <algorithm>

using namespace std;
using namespace cell_world;

Map_editor::Map_editor(World &world, ge211::Dimensions scene_dimensions) :
world(world),
_scene_dimensions(scene_dimensions),
_cell_group(world.create_cell_group()),
_visibility(Visibility::create_graph(_cell_group)),
_view(_cell_group, scene_dimensions)
{
    Cell_group selected = world.create_cell_group( world.name + "_gates");
    //_sub_worlds.reset(selected,_world_connections);

    _gates = Graph(selected);
    auto world_graph = world.create_graph();
    _sub_worlds = world_graph.get_sub_graphs(_gates);

    Cell_group_view bridges {selected,Blue,true};
    cells_view.emplace_back(bridges);

    Cell_group_view visible_cells {{}, Yellow, false};
    cells_view.emplace_back(visible_cells);

    Cell_group_view sub_world_cells {{}, Purple, false};
    cells_view.emplace_back(sub_world_cells);

    Cell_group_view connected_gates_cells {{}, Green, false};
    cells_view.emplace_back(connected_gates_cells);

    _current_cell_id = Not_found;
    _current_sub_world = Not_found;
    _message_timer = 0;
    refresh_values();
    cout << "environment complexity: " << _visibility.get_entropy() << endl;
}

void Map_editor::on_mouse_move(ge211::Position mouse_position) {
    int32_t index = _view.get_cell(mouse_position);
    _update_current_cell(index);
}

void Map_editor::on_mouse_up(ge211::Mouse_button mouse_button, ge211::Position mouse_position) {
    int32_t index = _view.get_cell(mouse_position);
    if (index>=0) {
        if (mouse_button == ge211::Mouse_button::left) {
            world.set_occlusion(index, !world[index].occluded);
        }
        else {
            cells_view[_Gates].cells.toggle(world[index]);
        }
        refresh_values();
    }
}

ge211::Dimensions Map_editor::initial_window_dimensions() const {
    return _scene_dimensions;
}

void Map_editor::draw(ge211::Sprite_set &sprites) {
    _view.draw_editor(sprites,_current_cell_id, cells_view, _message);
}

void Map_editor::on_key_up(ge211::Key key) {
    char32_t c;
    c = key.code();
    switch(c){
        case 's':
            world.save();
            cells_view[_Gates].cells.save(world.name + "_gates");
            _message = "Saved to file";
            _message_timer = 0;
            break;
        case 'l':
            world.load();
            cells_view[_Gates].cells = world.create_cell_group(world.name + "_gates");
            refresh_values();
            _message = "Re-loaded from file";
            _message_timer = 0;
            break;
        case '1':
            _message = "Showing visibility";
            cells_view[_Visible].show = !cells_view[_Visible].show;
            _message_timer = 0;
            break;
        case '2':
            _message = "Showing sub_worlds";
            cells_view[_Sub_world].show = !cells_view[_Sub_world].show;
            _message_timer = 0;
            break;
        case '3':
            _message = "Showing gates";
            cells_view[_Gates].show = !cells_view[_Gates].show;
            _message_timer = 0;
            break;
        case '4':
            _message = "Showing connected gates";
            cells_view[_Connected_gates].show = !cells_view[_Connected_gates].show;
            _message_timer = 0;
            break;
        case 'r':
            _message = "Finding bridges";
            {
//                Cell_group ns = _sub_worlds.find_bridges(_world_connections);
//                cells_view[_Gates].cells = ns;
            }
            refresh_values();
            _message_timer = 0;
            break;
        default:
            _message = "Invalid command";
            _message_timer = 0;
            break;
    }
}

void Map_editor::on_frame(double dt) {
    _message_timer += dt;
    if (_message_timer > 2) _message = "";
}

void Map_editor::refresh_values() {
    L("Map_editor::refresh_values() start");

    Cell_group cg = world.create_cell_group();
    _visibility = Visibility::create_graph(cg);
    auto graph = world.create_graph();
//    _world_connections = world.connection_pattern.get_graph(cg);
//    _sub_worlds.reset(cells_view[_Gates].cells, _world_connections);
    auto c = Centrality::get_eigen_centrality(graph);
    double max = 0;
    for (uint32_t i = 0 ; i < graph.size(); i++) if (max < c[i]) max = c[i];
    for (uint32_t i = 0 ; i < graph.size(); i++) world[graph.nodes[i].id].value = c[i]/max;
    uint32_t index = _current_cell_id;
    _update_current_cell(Not_found);
    _update_current_cell(index);
    L("Map_editor::refresh_values() start");
}

void Map_editor::_update_current_cell(int32_t index) {
    L("Map_editor::_update_current_cell(int32_t ) start");
    if (index != Not_found) {
        if (index != _current_cell_id){
            _current_cell_id = index;
            if (world[index].occluded) {
                cells_view[_Visible].cells.clear();
                cells_view[_Sub_world].cells.clear();
                cells_view[_Connected_gates].cells.clear();
            } else {
                cells_view[_Visible].cells = _visibility[world[_current_cell_id]];
                if (_gates.nodes.contains(world[_current_cell_id])){
                    cells_view[_Connected_gates].cells = _gates[world[_current_cell_id]];
                } else {
                    cells_view[_Connected_gates].cells.clear();
                }
                    /*
                int32_t sub_world_index = _sub_worlds.get_sub_world_index(_current_cell_id);
                if (sub_world_index != _current_sub_world) {
                    _current_sub_world = sub_world_index;
                    if (sub_world_index == Sub_worlds::Occluded) {
                    } else if (sub_world_index == Sub_worlds::Is_gate) {
                        cells_view[_Connected_gates].cells.clear();
                        for (auto & gc : _sub_worlds.gate_by_cell_id(_current_cell_id).gate_connections) {
                            cells_view[_Connected_gates].cells.add(gc.destination);
                        }
                        cells_view[_Sub_world].cells.clear();
                        for (auto & gc : _sub_worlds.gate_by_cell_id(_current_cell_id).gate_connections) {
                            cells_view[_Sub_world].cells += gc.sub_world.cells;
                        }
                    } else {
                        cells_view[_Connected_gates].cells = _sub_worlds[_current_sub_world].gate_cells;
                        cells_view[_Sub_world].cells = _sub_worlds[_current_sub_world].cells;
                    }
                }*/
            }
        }
    } else {
        _current_cell_id = Not_found;
        _current_sub_world = Not_found;
        cells_view[_Visible].cells.clear();
        cells_view[_Sub_world].cells.clear();
        cells_view[_Connected_gates].cells.clear();
    }
    L("Map_editor::_update_current_cell(int32_t ) end");
}