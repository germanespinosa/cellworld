#include<controller.h>
#include <iomanip>
#include <iostream>
#include <string>     // std::string, std::to_string

using namespace ge211;
using namespace std;
using namespace cellworld;



Controller::Controller (World &world, std::vector<Agent*> &agents, const ge211::Dimensions scene_dimensions, uint16_t iterations, uint32_t  episodes) :
        _model(world, agents),
        _scene_dimensions(scene_dimensions),
        _view(world, scene_dimensions),
        _iterations(iterations),
        _episodes(episodes){
    episode = 0;
    _model.start_episode();
}

Controller::Controller (World &world, std::vector<Agent*> &agents, const ge211::Dimensions scene_dimensions ) :
        Controller (world, agents, scene_dimensions,0,1){}

void Controller::on_frame(double dt)
{
    static double t=0;
    t+=dt;
    if (t>.05){
        t=0;
        if (!_model.update() ||
            (_iterations && _model.state.iteration >= _iterations)) {
            _model.end_episode();
            episode++;
            if (episode == _episodes)
                quit();
            else
                _model.start_episode();
        }
    }
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
    _view.draw(sprites, _model.get_agents_data(), to_string(episode) + "-" + to_string(_model.state.iteration));
}
