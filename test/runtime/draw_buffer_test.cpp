#include <catch2/catch.hpp>
#include <dark/core/draw_buffer.hpp>

TEST_CASE("testing basic_draw_data_t", "[basic_draw_data_t construction]"){
    amt::core::basic_draw_data_t<char> d0( {amt::b_bit8(200), amt::f_bit8(0)}, 'a' );
    REQUIRE(d0.attributes.str() == "\x1b[38;5;0;48;5;200m");
    REQUIRE(d0.pixel == 'a');
    REQUIRE(d0.z_index == 0);

    amt::core::basic_draw_data_t<char> d1( {amt::b_bit8(200), amt::f_bit8(0)}, 'a', 1 );
    REQUIRE(d1.attributes.str() == "\x1b[38;5;0;48;5;200m");
    REQUIRE(d1.pixel == 'a');
    REQUIRE(d1.z_index == 1);
}

TEST_CASE("testing draw_buffer_t", "[draw_buffer_t construction]"){
    amt::draw_buffer_t d0;
    CHECK(d0.width() >= 0);
    CHECK(d0.height() >= 0);

    amt::draw_buffer_t d1(10,10);
    CHECK( ( d1.width() == 10 ) );
    CHECK( ( d1.height() == 10 ) );

    amt::draw_buffer_t d2(std::make_pair(10,10));
    CHECK( ( d2.width() == 10 ) );
    CHECK( ( d2.height() == 10 ) );

    // amt::draw_buffer d3 {
    //     std::make_pair( amt::basic_draw_data_t{ {amt::b_bit8(200), amt::f_bit8(0)}, 'a', 1 }, amt::point_t{0,0} ),
    //     std::make_pair( amt::basic_draw_data_t{ {amt::b_bit8(200), amt::f_bit8(0)}, 'a', 1 }, amt::point_t{1,0} )
    // };

    // auto& dd0 = d3.at(0,0);
    // (void)dd0;
    // CHECK( d3.at(0,0).attributes.str() == "\x1b[38;5;0;48;5;200m" );
    
}
