#pragma once
#include <ge211.h>
#include <model.h>
#include <view.h>
#include <connection.h>
#include <visibility.h>
#include <sub_world.h>

namespace cell_world {
    class Map_editor  : public ge211::Abstract_game {
        public:
        Map_editor(World &, Cell_group &, ge211::Dimensions, const Connection_pattern &);
        ge211::Dimensions initial_window_dimensions() const override;
        void draw(ge211::Sprite_set&) override;
        World &world;
        void on_mouse_move(ge211::Position) override;
        void on_mouse_up(ge211::Mouse_button, ge211::Position) override;
        void on_key_up(ge211::Key) override;
        void on_frame(double) override;
        void refresh_values();

        private:
        const uint32_t gates = 0;
        const uint32_t visible = 1;
        const uint32_t sub_world = 2;
        const uint32_t connected_gates = 3;

        void _update_current_cell(int32_t);

        ge211::Dimensions _scene_dimensions;
        std::string _message;
        double _message_timer;
        View _view;
        Visibility _visibility;
        Connection_pattern _connection_pattern;
        Connections _world_connections;
        int32_t _current_cell_id;
        int32_t _current_sub_world;
        Sub_worlds _sub_worlds;
        std::vector<Cell_group_view> cells_view;
    };
}