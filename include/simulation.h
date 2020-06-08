#pragma once
#include <model.h>
#include <view.h>

namespace cell_world{
    class Simulation : public ge211::Abstract_game
    {
        public:
        Simulation (Model &, ge211::Dimensions);
        Simulation (Model &, ge211::Dimensions, unsigned int);
        ge211::Dimensions initial_window_dimensions() const override;
        void draw(ge211::Sprite_set&) override;
        bool is_paused = false;
        void on_key(ge211::Key) override;
        void on_frame(double) override;
        void on_start() override;
        void on_quit() override;
        void run_silent();
        void run_silent(bool);
        unsigned int episode;

        private:
        ge211::Dimensions _scene_dimensions;
        Model &_model;
        View _view;
        unsigned int _episodes;
    };
}