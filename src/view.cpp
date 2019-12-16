#include<view.h>
#include<iostream>

using namespace ge211;
using namespace std;
using namespace cellworld;

View::View(World &world, Dimensions scene_dimensions):
    _world(world) , 
    _scene_dimensions(scene_dimensions)
{
    double sx,sy,lx,ly;
    lx = sx = _world[0].location.x;
    ly = sy = _world[0].location.y;
    for (unsigned int i=0;i<_world.size();i++){
        double x = _world[i].location.x;
        double y = _world[i].location.y;
        if (lx < x) lx = x;
        if (ly < y) ly = y;
        if (sx > x) sx = x;
        if (sy > y) sy = y;
    }
    double rx = (_scene_dimensions.width - 100) / (lx - sx);
    double ry = (_scene_dimensions.height - 100) / (ly - sy);
    _ratio = rx<ry?rx:ry;
    _cell_size = (int)_ratio/2-1;
    _cell_sprites.emplace_back(_cell_size, ge211::Color{0,0,0}); //0
    _cell_sprites.emplace_back(_cell_size, ge211::Color{255,255,255}); //1
    _cell_sprites.emplace_back(_cell_size, ge211::Color{255,0,0}); //2
    _cell_sprites.emplace_back(_cell_size, ge211::Color{0,255,0}); //3
    _cell_sprites.emplace_back(_cell_size, ge211::Color{0,0,255}); //4
    _cell_sprites.emplace_back(_cell_size, ge211::Color{255,255,0}); //5
    _cell_sprites.emplace_back(_cell_size, ge211::Color{0,255,255}); //6
    _cell_sprites.emplace_back(_cell_size, ge211::Color{255,0,255}); //7
    _cell_sprites.emplace_back(_cell_size, ge211::Color{192,192,192}); //8
    _cell_sprites.emplace_back(_cell_size, ge211::Color{128,128,128}); //9
    _cell_sprites.emplace_back(_cell_size, ge211::Color{128,0,0}); //10
    _cell_sprites.emplace_back(_cell_size, ge211::Color{128,128,0}); //11
    _cell_sprites.emplace_back(_cell_size, ge211::Color{0,128,0}); //12
    _cell_sprites.emplace_back(_cell_size, ge211::Color{128,0,128}); //13
    _cell_sprites.emplace_back(_cell_size, ge211::Color{0,128,128}); //14
    _cell_sprites.emplace_back(_cell_size, ge211::Color{0,0,128}); //15
}

Basic_position<int> View::_screen_location (const Basic_position<double> &location)
{
    return {
        (int)(_scene_dimensions.width / 2 + location.x * _ratio - _ratio  / 2), 
        (int)(_scene_dimensions.height / 2 + location.y * _ratio - _ratio  / 2)
    };
}

void View::draw(Sprite_set& sprites, vector<Agent_data> agents, string text)
{
    fps.reconfigure(Text_sprite::Builder(sans) << text);
    sprites.add_sprite(fps, {10, 10});
    for (unsigned int i =0 ; i< _world.size(); i++) {
        const Cell &cell = _world[i];
        if (cell.occluded){
            sprites.add_sprite(_cell_sprites[9], _screen_location(cell.location),0);
        } else{
            sprites.add_sprite(_cell_sprites[1], _screen_location(cell.location),0);
        }
    }
    for (unsigned int i =0 ; i< agents.size(); i++) {
        sprites.add_sprite(_cell_sprites[agents[i].color], _screen_location(_world[agents[i].cell_id].location),1);
    }
}