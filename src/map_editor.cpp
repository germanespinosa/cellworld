//
// Created by german on 12/26/19.
//

#include "map_editor.h"
#include <iostream>
#include <algorithm>
using namespace std;

namespace cellworld{
    Map_editor::Map_editor(cellworld::World &world, ge211::Dimensions scene_dimensions) :
    world(world),
    _scene_dimensions(scene_dimensions),
    _view(world, scene_dimensions)
    {}

    void Map_editor::on_mouse_move(ge211::Position mouse_position) {
        _mouse_position = mouse_position;
    }

    void Map_editor::on_mouse_up(ge211::Mouse_button mouse_button, ge211::Position mouse_position) {
        int32_t index = _view.get_cell(mouse_position);
        if (index>=0) {
            if (mouse_button == ge211::Mouse_button::left)
                world.set_occlusion(index,!world[index].occluded);
            else {
                std::vector<uint32_t>::iterator it;

                it = find (_selected_cells.begin(), _selected_cells.end(), 30);
                if (it == _selected_cells.end())
                    _selected_cells.push_back(index);
                else
                    _selected_cells.erase(it);

            }
        }
    }

    ge211::Dimensions Map_editor::initial_window_dimensions() const {
        return _scene_dimensions;
    }

    void Map_editor::draw(ge211::Sprite_set &sprites) {
        _view.draw_editor(sprites,_mouse_position, _selected_cells, _message);
    }

    void Map_editor::on_key_up(ge211::Key key) {
        char32_t c;
        c = key.code();
        //cout << c << endl;
        switch(c){
            case 's':
                world.save();
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

}