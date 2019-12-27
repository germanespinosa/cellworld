//
// Created by german on 12/26/19.
//

#ifndef CELLWORLD_MAP_EDITOR_H
#define CELLWORLD_MAP_EDITOR_H


#include <ge211.h>
#include <model.h>
#include <view.h>
namespace cellworld {
    class Map_editor  : ge211::Abstract_game {
        Map_editor(World &, ge211::Dimensions );
        ge211::Dimensions initial_window_dimensions() const override;
        void draw(ge211::Sprite_set&) override;
        World &world;
        void on_mouse_move(ge211::Position) override;
        void on_mouse_up(ge211::Mouse_button, ge211::Position) override;
        ge211::Position _mouse_position{0,0};
        ge211::Dimensions _scene_dimensions;
        View _view;

    };
}

#endif //CELLWORLD_MAP_EDITOR_H
