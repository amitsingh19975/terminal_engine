#include <iostream>
#include <vector>
#include "dark/terminal.hpp"
#include <unistd.h>
#include <dark/core/text.hpp>

using namespace std::chrono_literals;

struct layer : amt::layer_t{
    layer(int const& w, int const& h)
        : layer_t("My Layer")
        , w(w)
        , h(h)
    {}

    void on_update(amt::timestamp_t ts, amt::draw_buffer_t& d) override{
        time += ts;
        if( time >= 10.f ){
            time = 0;
            x += inx;
            y += iny;
            if( x >= w - 1 || x <= 0 ) inx = -inx;
            if( y >= h - 1 || y <= 0 ) iny = -iny;

            n = ( n + 1 ) % 10;

            auto p = std::make_pair( 
                    amt::draw_data_t{ {amt::f_bit8(200), amt::escape_codes::UNDERLINE}, static_cast<char> ( 'a' + n ), 1 }, 
                    amt::core::point_t(x,y) 
                );
            d << p;
        }
    }

    void on_event(amt::event_base_t& e) override {
        amt::event_dispatcher_t dis(e);
        
        dis.dispatch<amt::key_pressed_event_t>([&](amt::key_pressed_event_t&){
            return false;
        });
    }

    int x {0};
    int y {0};
    int inx{1};
    int iny{1};
    int const& w;
    int const& h;
    int n = 0;
    float time{0.f};
};

struct App : amt::terminal_t{

    App(){
        push_layer( std::make_shared<layer>(width(),height()) );
    }

    virtual void on_window_close(amt::key_pressed_event_t& e) noexcept override{
        if(e.get_key_code() == amt::Keyboard_t::KEY_ESC || e.get_key_code() == amt::Keyboard_t::KEY_q ){
            stop();
        }
    }

};

int main(){

    App a;
    a.run();
    return 0;
}