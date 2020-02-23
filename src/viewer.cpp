#include <viewer.h>
using namespace std;
namespace cell_world {
    Viewer::Viewer(const Cell_group& cells, Viewer_handler &handler) :
    _view(cells,{1024,768}),
    _handler(handler),
    _cells(cells){

    }
    void Viewer::on_mouse_down(ge211::Mouse_button b, ge211::Position p) {
        Location l {(double)p.x,(double)p.y};
        _handler.on_mouse_down(b,l);
    }

    void Viewer::on_mouse_up(ge211::Mouse_button b, ge211::Position p) {
        Location l {(double)p.x,(double)p.y};
        _handler.on_mouse_up(b,l);
    }

    void Viewer::on_mouse_move(ge211::Position p) {
        Location l {(double)p.x,(double)p.y};
        _handler.on_mouse_move(l);
    }

    void Viewer::on_key_up(ge211::Key k) {
        _handler.on_key_up(k);
    }

    void Viewer::on_key_down(ge211::Key k) {
        _handler.on_key_down(k);
    }

    ge211::Dimensions Viewer::initial_window_dimensions() const {
        return {1024,768};
    }

    void Viewer::draw(ge211::Sprite_set &sprites) {
        _view.draw_editor(sprites,Not_found, _handler.cells_view,_handler.message);
    }

    void Viewer::on_frame(double d) {
        _handler.on_frame(d);
    }
}