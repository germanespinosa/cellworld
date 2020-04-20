#include <simulation.h>
#include <string>
#include <iostream>

using namespace ge211;
using namespace std;
using namespace cell_world;

Simulation::Simulation (Model &model, const ge211::Dimensions scene_dimensions, uint32_t  episodes) :
        _scene_dimensions(scene_dimensions),
        _model(model),
        _view(model.cells, scene_dimensions),
        _episodes(episodes){
    episode = 0;
}

Simulation::Simulation (Model &model, const ge211::Dimensions scene_dimensions ) :
        Simulation (model, scene_dimensions,1){}

void Simulation::on_frame(double )
{
    if (_model.status == Model::Status::Idle ) _model.start_episode();
    else if ( _model.status == Model::Status::Running && !_model.try_update() && _model.finished ) {
        _model.end_episode();
        episode++;
        if (episode == _episodes)
            quit();
        else
            _model.start_episode();
    }
}

void Simulation::on_key(Key key)
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

Dimensions Simulation::initial_window_dimensions() const
{
    return _scene_dimensions;
}

void Simulation::draw(Sprite_set& sprites)
{
    _view.draw_scene(sprites, _model.get_agents_data(), to_string(episode) + "-" + to_string(_model.iteration));
}

void Simulation::run_silent() {
    run_silent(false);
}

void Simulation::run_silent(bool show_progress) {
    string bar ("|--------------------------------------------------|  ");
    uint32_t step = _episodes / 50;
    uint32_t step_count = 0;
    uint32_t progress = 0;
    for (uint32_t i = 0; i < _episodes; i++){
        if (show_progress) {
            step_count++;
            if (step_count >= step){
                step_count=0;
                progress++;
                bar[progress]='=';
                bar[progress+1]='>';
                if (show_progress) cout << "\r" << bar << (progress<5?" ":"") << progress * 2 << "% (" << (i+1) << "/" << _episodes << ")" << flush;
            }
        }
        _model.start_episode();
        while (_model.update());
        _model.end_episode();
    }
    if (show_progress) cout << "\r|==================================================| 100% (" << _episodes << "/" << _episodes << ")" << flush;
}

void Simulation::on_start() {
}

void Simulation::on_quit() {
    if (_model.status==Model::Status::Running) _model.end_episode();
}
