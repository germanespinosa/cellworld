#pragma once
#include <model.h>
#include <view.h>

namespace cell_world{

    using Key = ge211::Key;
    using Mouse_button = ge211::Mouse_button;

    struct Viewer_handler {
        virtual void on_mouse_down(Mouse_button, Location,  int) {};
        virtual void on_mouse_move(Location,  int) {};
        virtual void on_mouse_up(Mouse_button, Location,  int) {};
        virtual void on_key_up(Key) {};
        virtual void on_key_down(Key) {};
        virtual void on_frame(double){};
        std::vector<Cell_group_view> cells_view;
        std::string message;
    };

    struct Viewer : public ge211::Abstract_game {
        Viewer(const Cell_group&, Viewer_handler &);
        ge211::Dimensions initial_window_dimensions() const override;
        void on_mouse_down(ge211::Mouse_button, ge211::Position) override;
        void on_mouse_move(ge211::Position) override;
        void on_mouse_up(ge211::Mouse_button, ge211::Position) override;
        void on_key_up(ge211::Key) override;
        void on_key_down(ge211::Key) override;
        void draw(ge211::Sprite_set&) override;
        void on_frame(double) override;
    private:
        View _view;
        Viewer_handler &_handler;
        Cell_group _cells;
    };
}