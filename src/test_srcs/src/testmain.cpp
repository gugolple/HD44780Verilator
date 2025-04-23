#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include "config.h"

static int factorial(int n) {
    if (n <= 1) {
        return n;
    }
    return n * factorial(n - 1);
}

TEST_CASE("Simple test case") {
    CHECK(factorial( 0) == 1);
    CHECK(factorial( 1) == 1);
    CHECK(factorial(1) == 1 );
    CHECK(factorial(10) == 3'628'800);
    CHECK(factorial(10) == 3628800 );
    CHECK(factorial(2) == 2 );
    CHECK(factorial(3) == 6 );
}

TEST_CASE("Initialization of HD44780") {
}


int main( int argc, char* argv[] ) {
    std::cout << "Commit string: " << VERSION << std::endl;
    // Entry point for all the library
    int result = Catch::Session().run( argc, argv );
    return result;
}
