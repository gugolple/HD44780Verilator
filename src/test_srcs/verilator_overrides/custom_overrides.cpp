#include "custom_overrides.h"

// STD Includes for functionality
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <fmt/format.h>
#include <string>
#include <sstream>

// Catch2 to do the mixing
#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

void my_catch2_print( const char * format, ...) {
    // Buffer
    std::array<char, 1024> formatted;
    // Open variadic macros
    va_list args;
    va_start(args, format);
    // Do conversion from variadic to string
    vsnprintf(formatted.data(), formatted.size(), format, args);
    // Close variadic macros
    va_end(args);
    UNSCOPED_INFO(std::string(formatted.data()));
}
 

int my_printf( const char * format, ... ) {
    int result = 0;
    va_list args;
    va_start(args, format);
    result = vprintf(format, args);
    va_end(args);
    return result;
}
