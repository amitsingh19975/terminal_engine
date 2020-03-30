#include <iostream>
#include <vector>
#include "core/draw_buffer.hpp"

int main(){
    amt::core::draw_buffer d = {
        { {amt::f_bit8(200), amt::escape_codes::ITALIC}, "hello" },
        { amt::core::draw_buffer::space },
        { {amt::b_bit8(200), amt::escape_codes::UNDERLINE}, "hi" },
    };

    d.push_back(amt::core::draw_buffer::space);
    d.push_back(
        { {amt::escape_codes::FAINT}, "test" }
    );

    d.draw();
    return 0;
}