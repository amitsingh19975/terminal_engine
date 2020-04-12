#include <catch2/catch.hpp>
#include <dark/core/color.hpp>

TEST_CASE("testing amt::core::detail::rgb", "[rgb]"){
    amt::core::detail::rgb r0;
    REQUIRE( r0.r == 0 );
    REQUIRE( r0.g == 0 );
    REQUIRE( r0.b == 0 );
    REQUIRE( r0.str() == "2;0;0;0m" );
    REQUIRE( r0.rstr() == "2;0;0;0" );

    amt::core::detail::rgb r1(200);
    REQUIRE( r1.r == 200 );
    REQUIRE( r1.g == 0 );
    REQUIRE( r1.b == 0 );
    REQUIRE( r1.str() == "2;200;0;0m" );
    REQUIRE( r1.rstr() == "2;200;0;0" );
    
    amt::core::detail::rgb r2(200,200);
    REQUIRE( r2.r == 200 );
    REQUIRE( r2.g == 200 );
    REQUIRE( r2.b == 0 );
    REQUIRE( r2.str() == "2;200;200;0m" );
    REQUIRE( r2.rstr() == "2;200;200;0" );
    
    amt::core::detail::rgb r3(200,200,200);
    REQUIRE( r3.r == 200 );
    REQUIRE( r3.g == 200 );
    REQUIRE( r3.b == 200 );
    REQUIRE( r3.str() == "2;200;200;200m" );
    REQUIRE( r3.rstr() == "2;200;200;200" );
}

TEST_CASE("testing amt::core::detail::bit8", "[bit8]"){
    amt::core::detail::bit8 r0;
    REQUIRE( r0.col == 0 );
    REQUIRE( r0.str() == "5;0m" );
    REQUIRE( r0.rstr() == "5;0" );
    
    amt::core::detail::bit8 r1(200);
    REQUIRE( r1.col == 200 );
    REQUIRE( r1.str() == "5;200m" );
    REQUIRE( r1.rstr() == "5;200" );
}

TEST_CASE("testing f_bit8", "[f_bit8]"){
    amt::f_bit8 r0;
    REQUIRE( r0.col == 0 );
    REQUIRE( r0.str() == "\x1b[38;5;0m" );
    REQUIRE( r0.rstr() == "38;5;0" );
    
    amt::f_bit8 r1(200);
    REQUIRE( r1.col == 200 );
    REQUIRE( r1.str() == "\x1b[38;5;200m" );
    REQUIRE( r1.rstr() == "38;5;200" );
}

TEST_CASE("testing b_bit8", "[b_bit8]"){
    amt::b_bit8 r0;
    REQUIRE( r0.col == 0 );
    REQUIRE( r0.str() == "\x1b[48;5;0m" );
    REQUIRE( r0.rstr() == "48;5;0" );
    
    amt::b_bit8 r1(200);
    REQUIRE( r1.col == 200 );
    REQUIRE( r1.str() == "\x1b[48;5;200m" );
    REQUIRE( r1.rstr() == "48;5;200" );
}

TEST_CASE("testing f_rgb", "[f_rgb]"){
    amt::f_rgb r0;
    REQUIRE( r0.r == 0 );
    REQUIRE( r0.g == 0 );
    REQUIRE( r0.b == 0 );
    REQUIRE( r0.str() == "\x1b[38;2;0;0;0m" );
    REQUIRE( r0.rstr() == "38;2;0;0;0" );

    amt::f_rgb r1(200);
    REQUIRE( r1.r == 200 );
    REQUIRE( r1.g == 0 );
    REQUIRE( r1.b == 0 );
    REQUIRE( r1.str() == "\x1b[38;2;200;0;0m" );
    REQUIRE( r1.rstr() == "38;2;200;0;0" );
    
    amt::f_rgb r2(200,200);
    REQUIRE( r2.r == 200 );
    REQUIRE( r2.g == 200 );
    REQUIRE( r2.b == 0 );
    REQUIRE( r2.str() == "\x1b[38;2;200;200;0m" );
    REQUIRE( r2.rstr() == "38;2;200;200;0" );
    
    amt::f_rgb r3(200,200,200);
    REQUIRE( r3.r == 200 );
    REQUIRE( r3.g == 200 );
    REQUIRE( r3.b == 200 );
    REQUIRE( r3.str() == "\x1b[38;2;200;200;200m" );
    REQUIRE( r3.rstr() == "38;2;200;200;200" );
}

TEST_CASE("testing b_rgb", "[b_rgb]"){
    amt::b_rgb r0;
    REQUIRE( r0.r == 0 );
    REQUIRE( r0.g == 0 );
    REQUIRE( r0.b == 0 );
    REQUIRE( r0.str() == "\x1b[48;2;0;0;0m" );
    REQUIRE( r0.rstr() == "48;2;0;0;0" );

    amt::b_rgb r1(200);
    REQUIRE( r1.r == 200 );
    REQUIRE( r1.g == 0 );
    REQUIRE( r1.b == 0 );
    REQUIRE( r1.str() == "\x1b[48;2;200;0;0m" );
    REQUIRE( r1.rstr() == "48;2;200;0;0" );
    
    amt::b_rgb r2(200,200);
    REQUIRE( r2.r == 200 );
    REQUIRE( r2.g == 200 );
    REQUIRE( r2.b == 0 );
    REQUIRE( r2.str() == "\x1b[48;2;200;200;0m" );
    REQUIRE( r2.rstr() == "48;2;200;200;0" );
    
    amt::b_rgb r3(200,200,200);
    REQUIRE( r3.r == 200 );
    REQUIRE( r3.g == 200 );
    REQUIRE( r3.b == 200 );
    REQUIRE( r3.str() == "\x1b[48;2;200;200;200m" );
    REQUIRE( r3.rstr() == "48;2;200;200;200" );
}

TEST_CASE("testing color","[color]"){
    amt::color c0;
    REQUIRE(c0.empty() == true);

    amt::b_rgb br(200,200,200);
    amt::color c1(br);
    auto temp_br = c1.get<amt::b_rgb>();
    REQUIRE( c1.empty() == false );
    REQUIRE( temp_br.r == 200 );
    REQUIRE( temp_br.g == 200 );
    REQUIRE( temp_br.b == 200 );
    REQUIRE( c1.str() == "\x1b[48;2;200;200;200m" );
    REQUIRE( c1.rstr() == "48;2;200;200;200" );
    
    amt::f_rgb fr(200,200,200);
    amt::color c2(fr);
    auto temp_fr = c2.get<amt::f_rgb>();
    REQUIRE( c2.empty() == false );
    REQUIRE( temp_fr.r == 200 );
    REQUIRE( temp_fr.g == 200 );
    REQUIRE( temp_fr.b == 200 );
    REQUIRE( c2.str() == "\x1b[38;2;200;200;200m" );
    REQUIRE( c2.rstr() == "38;2;200;200;200" );

    amt::b_bit8 bb(200);
    amt::color c3(bb);
    auto temp_bb = c3.get<amt::b_bit8>();
    REQUIRE( c3.empty() == false );
    REQUIRE( temp_bb.col == 200 );
    REQUIRE( c3.str() == "\x1b[48;5;200m" );
    REQUIRE( c3.rstr() == "48;5;200" );

    amt::f_bit8 fb(200);
    amt::color c4(fb);
    auto temp_fb = c4.get<amt::f_bit8>();
    REQUIRE( c4.empty() == false );
    REQUIRE( temp_fb.col == 200 );
    REQUIRE( c4.str() == "\x1b[38;5;200m" );
    REQUIRE( c4.rstr() == "38;5;200" );

}