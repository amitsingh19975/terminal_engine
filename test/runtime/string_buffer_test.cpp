#include <catch2/catch.hpp>
#include <dark/core/string_buffer.hpp>
#include <unistd.h>
#include <string>

TEST_CASE("testing eager writing in string_buff", "[eager_write]"){

    int p[2];
    REQUIRE( pipe(p) >= 0 );


    int f = fork();

    if( f > 0){
        close(1);
        close(p[0]);
        dup2(p[1], 1);
        amt::string_buff.eager_write("hello world");
        REQUIRE(amt::string_buff.buf() == "");
    }else{
        char buff[2048];
        close(p[1]);
        auto len = read(p[0],buff,2048);
        REQUIRE( strncmp(buff,"hello world",static_cast<size_t>(len)) == 0);
    }
}

TEST_CASE("testing lazy writing in string_buff", "[lazy_write]"){

    amt::string_buff<<"hello world";
    REQUIRE(amt::string_buff.buf() == "hello world");

    int p[2];
    REQUIRE( pipe(p) >= 0 );


    int f = fork();

    if( f > 0){
        close(1);
        close(p[0]);
        dup2(p[1], 1);
        amt::string_buff.flush();
        REQUIRE(amt::string_buff.buf() == "");

    }else{
        char buff[2048];
        close(p[1]);
        auto len = read(p[0],buff,2048);
        REQUIRE( strncmp(buff,"hello world",static_cast<size_t>(len)) == 0);
    }
}
