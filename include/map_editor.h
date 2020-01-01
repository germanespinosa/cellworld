#pragma once
#include <ge211.h>
#include <model.h>
#include <view.h>
#include <connection.h>
#include <visibility.h>
namespace cellworld {
    struct Map_editor  : ge211::Abstract_game {
        Map_editor(World &, ge211::Dimensions, std::vector<Coordinates> );
        ge211::Dimensions initial_window_dimensions() const override;
        void draw(ge211::Sprite_set&) override;
        World &world;
        void on_mouse_move(ge211::Position) override;
        void on_mouse_up(ge211::Mouse_button, ge211::Position) override;
        void on_key_up(ge211::Key) override;
        void on_frame(double) override;
        void refresh_values();
    private:
        ge211::Position _mouse_position{0,0};
        ge211::Dimensions _scene_dimensions;
        std::string _message;
        double _message_timer;
        View _view;
        Connections _world_connections;
        Visibility _visibility;
        std::vector<Coordinates> _connection_pattern;
        int32_t _current_cell_id;
        Cell_group _selected_cells;
        Cell_group _visible_cells;
    };
}