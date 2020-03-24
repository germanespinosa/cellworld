#pragma once
#include <model.h>
#include <view.h>

namespace cell_world{
    class Simulation : public ge211::Abstract_game
    {
        public:
        Simulation (Model &, ge211::Dimensions);
        Simulation (Model &, ge211::Dimensions, uint16_t);
        Simulation (Model &, ge211::Dimensions, uint16_t, uint32_t);
        ge211::Dimensions initial_window_dimensions() const override;
        void draw(ge211::Sprite_set&) override;
        bool is_paused = false;
        void on_key(ge211::Key) override;
        void on_frame(double) override;
        void on_start() override;
        void run_silent();
        void run_silent(bool);
        uint32_t episode;

        private:
        ge211::Dimensions _scene_dimensions;
        Model &_model;
        View _view;
        uint16_t _iterations;
        uint32_t _episodes;
    };
}