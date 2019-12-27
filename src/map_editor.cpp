//
// Created by german on 12/26/19.
//

#include "map_editor.h"

namespace cellworld{
    Map_editor::Map_editor(cellworld::World &world, ge211::Dimensions scene_dimensions) :
    world(world),
    _view(world, scene_dimensions)
    {}

    void Map_editor::on_mouse_move(ge211::Position mouse_position) {
        _mouse_position = mouse_position;
    }

    void Map_editor::on_mouse_up(ge211::Mouse_button mouse_button, ge211::Position mouse_position) {
        Abstract_game::on_mouse_up( mouse_button, mouse_position);
    }

    ge211::Dimensions Map_editor::initial_window_dimensions() const {
        return _scene_dimensions;
    }

    void Map_editor::draw(ge211::Sprite_set &sprites) {
        _view.draw_editor(sprites,_mouse_position);
    }

}