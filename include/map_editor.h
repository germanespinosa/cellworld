#pragma once
#include <model.h>
#include <view.h>
#include <connection.h>
#include <visibility.h>
#include <graph.h>

namespace cell_world {
    class Map_editor : public ge211::Abstract_game {
        public:
        Map_editor(World &, ge211::Dimensions);
        ge211::Dimensions initial_window_dimensions() const override;
        void draw(ge211::Sprite_set&) override;
        World &world;
        void on_mouse_move(ge211::Position) override;
        void on_mouse_up(ge211::Mouse_button, ge211::Position) override;
        void on_key_up(ge211::Key) override;
        void on_frame(double) override;
        void refresh_values();
        private:
        const unsigned int _Gates = 0;
        const unsigned int _Visible = 1;
        const unsigned int _Sub_world = 2;
        const unsigned int _Connected_gates = 3;
        void _update_current_cell( int);
        ge211::Dimensions _scene_dimensions;
        std::string _message;
        double _message_timer;
        Cell_group _cell_group;
        Graph _visibility;
        View _view;
         int _current_cell_id;
         int _current_sub_world;
        std::vector<Cell_group_view> cells_view;
        std::vector<Graph> _sub_worlds;
        Graph _gates;
    };
}