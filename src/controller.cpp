#include<controller.h>
#include <iomanip>
#include <iostream>
#include <string>     // std::string, std::to_string

using namespace ge211;
using namespace std;
using namespace cellworld;

Controller::Controller (World &world, std::vector<Agent*> &agents, const ge211::Dimensions scene_dimensions ) : 
    _model(world, agents),
    _scene_dimensions(scene_dimensions), 
    _view(world, scene_dimensions)
    {}

void Controller::on_frame(double dt)
{
    _model.update();
}

void Controller::on_key(Key key)
{
    if (key == Key::code('q')) {
        quit();
    } else if (key == Key::code('f')) {
        get_window().set_fullscreen(!get_window().get_fullscreen());
    } else if (key == Key::code('p')) {
        is_paused = !is_paused;
    } else if (key == Key::code(' ') && !is_paused) {
    }
}

Dimensions Controller::initial_window_dimensions() const
{
    return _scene_dimensions;
}

void Controller::draw(Sprite_set& sprites)
{
    _view.draw(sprites, _model.get_agents_data(), to_string(get_frame_rate()));
}
