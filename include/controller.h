#pragma once
#include <ge211.h>
#include <model.h>
#include <view.h>
namespace cellworld{
    struct Controller : ge211::Abstract_game
    {
        Controller (World &, std::vector<Agent*> &, const ge211::Dimensions);
        ge211::Dimensions initial_window_dimensions() const override;
        void draw(ge211::Sprite_set&) override;
        bool is_paused = false;
        void on_key(ge211::Key) override;
        void on_frame(double) override;
        private:
        Model _model;
        ge211::Dimensions _scene_dimensions;
        View _view;
    };
}