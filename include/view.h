#pragma once
#include <ge211.h>
#include <core.h>
#include <world.h>
#include <cell_group.h>
#include <agent.h>

namespace cell_world{
    struct Cell_group_view{
        Cell_group cells;
        Color color;
        bool show;
    };
    struct View
    {
        View(World &world, ge211::Dimensions);
        void draw_scene (ge211::Sprite_set& , std::vector<Agent_data>, const std::string&);
        void draw_editor (ge211::Sprite_set&, int32_t , std::vector<Cell_group_view>, const std::string&);
        int32_t get_cell(ge211::Position);
        private:
        void _draw_world(ge211::Sprite_set& sprites);
        ge211::Basic_position<int> _screen_location (const ge211::Basic_position<double> & );
        World &_world;
        std::vector<ge211::Circle_sprite> _cell_sprites;
        std::vector<ge211::Circle_sprite> _value_sprites;
        ge211::Font sans{"sans.ttf", 30};
        ge211::Text_sprite fps;
        const ge211::Dimensions _scene_dimensions;
        double _ratio;
        int _cell_size;
    };
}