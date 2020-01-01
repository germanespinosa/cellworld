//
// Created by german on 12/26/19.
//

#include "map_editor.h"
#include <iostream>
#include <algorithm>
using namespace std;

namespace cellworld{
    Map_editor::Map_editor(cellworld::World &world, ge211::Dimensions scene_dimensions, std::vector<Coordinates> connection_pattern) :
    world(world),
    _scene_dimensions(scene_dimensions),
    _view(world, scene_dimensions),
    _visibility(world),
    _connection_pattern(connection_pattern),
    _selected_cells(world),
    _visible_cells(world)
    {
        cout << "Map_editor::Map_editor start" << endl;
        _current_cell_id = -1;
        _message_timer = 0;
        refresh_values();
        cout << "Map_editor::Map_editor end" << endl;
    }

    void Map_editor::on_mouse_move(ge211::Position mouse_position) {
        _mouse_position = mouse_position;
        int32_t index = _view.get_cell(mouse_position);
        if (index!=_current_cell_id){
            _visible_cells.clear();
            if (index>=0)
                for (uint32_t i = 0; i<world.size(); i++){
                    if (_visibility.is_visible(index,i)){
                        _visible_cells.add(i);
                    }
                }
            _current_cell_id = index;
        }
    }

    void Map_editor::on_mouse_up(ge211::Mouse_button mouse_button, ge211::Position mouse_position) {
        int32_t index = _view.get_cell(mouse_position);
        if (index>=0) {
            if (mouse_button == ge211::Mouse_button::left) {
                world.set_occlusion(index, !world[index].occluded);
                refresh_values();
            }
            else {
                _selected_cells.toggle(index);
            }
        }
    }

    ge211::Dimensions Map_editor::initial_window_dimensions() const {
        return _scene_dimensions;
    }

    void Map_editor::draw(ge211::Sprite_set &sprites) {
        _view.draw_editor(sprites,_current_cell_id, _selected_cells, _visible_cells, _message);
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

        }
    }

    void Map_editor::on_frame(double dt) {
        _message_timer += dt;
        if (_message_timer > 2) _message = "";
    }

    void Map_editor::refresh_values() {
        _visibility.reset();
        world.get_connections(_world_connections,_connection_pattern);
        _world_connections.process_eigen_centrality();
        double max = 0;
        for (uint32_t i = 0 ; i < _world_connections.size(); i++) if (max < _world_connections[i].eigen_centrality) max = _world_connections[i].eigen_centrality;
        for (uint32_t i = 0 ; i < _world_connections.size(); i++) world.set_value(i,_world_connections[i].eigen_centrality/max);
    }

}