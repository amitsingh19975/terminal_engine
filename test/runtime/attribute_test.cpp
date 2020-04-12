#include <catch2/catch.hpp>
#include <dark/core/attribute.hpp>
#include <dark/core/escape_codes.hpp>
#include <dark/core/color.hpp>

TEST_CASE("checking attr construction", "[Constructor]"){

    amt::attr a0;
    amt::attr a1 = { amt::f_bit8(200) };
    amt::attr a2 = { amt::b_bit8(200) };
    amt::attr a3 = { amt::f_rgb(200,200,200) };
    amt::attr a4 = { amt::b_rgb(200,200,200) };
    amt::attr a5 = { amt::escape_codes::ITALIC };
    amt::attr a6 = { amt::f_bit8(200), amt::escape_codes::ITALIC };
    amt::attr a7 = { amt::escape_codes::ITALIC, amt::f_bit8(200) };
    amt::attr a8 = { amt::escape_codes::ITALIC, amt::escape_codes::UNDERLINE, amt::f_bit8(200) };
    amt::attr a9 = { amt::escape_codes::ITALIC, amt::escape_codes::UNDERLINE, amt::f_bit8(200), amt::b_bit8(200) };

    REQUIRE(    a0.empty()  == true                             );
    REQUIRE(    a1.str()    == "\x1b[38;5;200m"                 );
    REQUIRE(    a2.str()    == "\x1b[48;5;200m"                 );
    REQUIRE(    a3.str()    == "\x1b[38;2;200;200;200m"         );
    REQUIRE(    a4.str()    == "\x1b[48;2;200;200;200m"         );
    REQUIRE(    a5.str()    == "\x1b[3m"                        );
    REQUIRE(    a6.str()    == "\x1b[38;5;200;3m"               );
    REQUIRE(    a7.str()    == "\x1b[38;5;200;3m"               );
    REQUIRE(    a8.str()    == "\x1b[38;5;200;4;3m"             );
    REQUIRE(    a9.str()    == "\x1b[48;5;200;38;5;200;4;3m"    );
}

TEST_CASE("checking attr operator<<", "[operator<<]"){

    amt::attr a0;
    REQUIRE( a0.empty()  == true );

    a0 << amt::f_bit8(200);
    REQUIRE( a0.str() == "\x1b[38;5;200m" );
    
    a0 << amt::b_rgb(200,200,200) ;
    REQUIRE( a0.str() == "\x1b[38;5;200;48;2;200;200;200m" );
    
    a0 << amt::escape_codes::ITALIC << amt::escape_codes::UNDERLINE ;
    REQUIRE( a0.str() == "\x1b[38;5;200;48;2;200;200;200;3;4m" );

}