//
// Created by german on 12/26/19.
//

#include "map_editor.h"
#include <iostream>
#include <algorithm>
using namespace std;

namespace cellworld{
    Map_editor::Map_editor(cellworld::World &world, Cell_group &selected, ge211::Dimensions scene_dimensions, std::vector<Coordinates> connection_pattern) :
    world(world),
    _scene_dimensions(scene_dimensions),
    _view(world, scene_dimensions),
    _visibility(world),
    _connection_pattern(connection_pattern),
    _selected_cells(selected),
    _visible_cells(world),
    _sub_world_cells(world)
    {
        Connections cnn;
        world.get_connections(cnn, connection_pattern);
        _sub_worlds.reset(world,selected,cnn);

        _current_cell_id = -1;
        _current_sub_world = -1;
        _message_timer = 0;
        _mode = 0;
        refresh_values();
    }

    void Map_editor::on_mouse_move(ge211::Position mouse_position) {
        _mouse_position = mouse_position;
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
                _selected_cells.toggle(index);
            }
            refresh_values();
        }
    }

    ge211::Dimensions Map_editor::initial_window_dimensions() const {
        return _scene_dimensions;
    }

    void Map_editor::draw(ge211::Sprite_set &sprites) {
        _view.draw_editor(sprites,_current_cell_id, _selected_cells, _mode == 0?_visible_cells:_sub_world_cells, _mode == 0?Yellow:Purple, _message);
    }

    void Map_editor::on_key_up(ge211::Key key) {
        char32_t c;
        c = key.code();
        switch(c){
            case 's':
                world.save();
                _world_connections.save(world.name + ".con");
                _selected_cells.save(world.name + ".sel");
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
                _message_timer = 0;
                _mode = 0;
                break;
            case '2':
                _message = "Showing subworlds";
                _message_timer = 0;
                _mode = 1;
                break;
            case 'r':
                _message = "Finding bridges";
                _message_timer = 0;
                Cell_group ns = _sub_worlds.find_bridges(world, _world_connections);
                _selected_cells = ns;
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
        _sub_worlds.reset(world,_selected_cells, _world_connections);
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
                    _visible_cells.clear();
                    _sub_world_cells.clear();
                } else {
                    _visibility.get_visible_cells(_visible_cells, _current_cell_id);
                    int32_t sub_world_index = _sub_worlds.get_sub_world_index(_current_cell_id);
                    if (sub_world_index != _current_sub_world) {
                        _current_sub_world = sub_world_index;
                        if (sub_world_index == Not_found) {
                            _sub_world_cells.clear();
                        }else if (sub_world_index == Sub_worlds::is_gate){
                            //Gate &gate = _sub_worlds.gate(_current_cell_id);
                            //cout << "gate: connected to " <<  gate._sub_world_ids.size() << endl;
                        } else {
                            _sub_worlds.get_cells(_sub_world_cells, _current_sub_world);
                        }
                    }
                }
            }
        } else {
            _current_cell_id = Not_found;
            _current_sub_world = Not_found;
            _visible_cells.clear();
            _sub_world_cells.clear();
        }
    }
}