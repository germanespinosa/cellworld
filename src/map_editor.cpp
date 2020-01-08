#include <map_editor.h>
#include <iostream>
#include <algorithm>

using namespace std;
using namespace cell_world;

Map_editor::Map_editor(World &world, Cell_group &selected, ge211::Dimensions scene_dimensions, const std::vector<Coordinates> &connection_pattern) :
world(world),
_scene_dimensions(scene_dimensions),
_view(world, scene_dimensions),
_visibility(world),
_connection_pattern(connection_pattern)
{
    Connections cnn;
    world.get_connections(cnn, connection_pattern);
    _sub_worlds.reset(world,selected,cnn);

    Cell_group_view bridges {selected,Blue,true};
    cells_view.emplace_back(bridges);

    Cell_group_view visible_cells {Cell_group(world), Yellow, true};
    cells_view.emplace_back(visible_cells);

    Cell_group_view sub_world_cells {Cell_group(world), Purple, true};
    cells_view.emplace_back(sub_world_cells);

    Cell_group_view connected_gates_cells {Cell_group(world), Green, true};
    cells_view.emplace_back(connected_gates_cells);

    _current_cell_id = Not_found;
    _current_sub_world = Not_found;
    _message_timer = 0;
    _mode = 0;
    refresh_values();
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
            cells_view[gates].cells.toggle(index);
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
            _world_connections.save(world.name + ".con");
            cells_view[gates].cells.save(world.name + ".sel");
            _message = "Saved to file";
            _message_timer = 0;
            break;
        case 'l':
            world.load();
            _message = "Re-loaded from file";
            _message_timer = 0;
            break;
        case '1':
            _message = "Showing visibility";
            cells_view[visible].show = !cells_view[visible].show;
            _message_timer = 0;
            break;
        case '2':
            _message = "Showing sub_worlds";
            cells_view[sub_world].show = !cells_view[sub_world].show;
            _message_timer = 0;
            break;
        case '3':
            _message = "Showing gates";
            cells_view[gates].show = !cells_view[gates].show;
            _message_timer = 0;
            break;
        case '4':
            _message = "Showing connected gates";
            cells_view[connected_gates].show = !cells_view[connected_gates].show;
            _message_timer = 0;
            break;
        case 'r':
            _message = "Finding bridges";
            _message_timer = 0;
            {
                Cell_group ns = _sub_worlds.find_bridges(world, _world_connections);
                cells_view[gates].cells = ns;
            }
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
    _visibility.reset();
    world.get_connections(_world_connections, _connection_pattern);
    _sub_worlds.reset(world,cells_view[gates].cells, _world_connections);
    _sub_worlds.reset_connections();
    _world_connections.process_eigen_centrality();
    double max = 0;
    for (uint32_t i = 0 ; i < _world_connections.size(); i++) if (max < _world_connections[i].eigen_centrality) max = _world_connections[i].eigen_centrality;
    for (uint32_t i = 0 ; i < _world_connections.size(); i++) world.set_value(i,_world_connections[i].eigen_centrality/max);
    uint32_t index = _current_cell_id;
    _update_current_cell(Not_found);
    _update_current_cell(index);
}

void Map_editor::_update_current_cell(int32_t index) {
    if (index != Not_found) {
        if (index != _current_cell_id){
            _current_cell_id = index;
            if (world[index].occluded) {
                cells_view[visible].cells.clear();
                cells_view[sub_world].cells.clear();
                cells_view[connected_gates].cells.clear();
            } else {
                _visibility.get_visible_cells(cells_view[visible].cells, _current_cell_id);
                int32_t sub_world_index = _sub_worlds.get_sub_world_index(_current_cell_id);
                if (sub_world_index != _current_sub_world) {
                    _current_sub_world = sub_world_index;
                    if (sub_world_index == Sub_worlds::Occluded) {
                    } else if (sub_world_index == Sub_worlds::Is_gate) {
                        cells_view[connected_gates].cells.clear();
                        for (auto & gc : _sub_worlds.gate_by_cell_id(_current_cell_id).gate_connections) {
                            cells_view[connected_gates].cells.add(gc.gate_id);
                        }
                        cells_view[sub_world].cells.clear();
                        for (unsigned int sw_id : _sub_worlds.gate_by_cell_id(_current_cell_id).sub_world_ids) {
                            Cell_group msw(world);
                            _sub_worlds.get_cells(msw, sw_id);
                            cells_view[sub_world].cells += msw;
                        }
                    } else {
                        cells_view[connected_gates].cells.clear();
                        _sub_worlds.get_cells(cells_view[sub_world].cells, _current_sub_world);
                    }
                }
            }
        }
    } else {
        _current_cell_id = Not_found;
        _current_sub_world = Not_found;
        cells_view[visible].cells.clear();
        cells_view[sub_world].cells.clear();
        cells_view[connected_gates].cells.clear();
    }
}
