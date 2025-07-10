#include "custom_overrides.h"

// STD Includes for functionality
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#include <fmt/format.h>
#include <string>
#include <sstream>
#include <memory>

// Catch2 to do the mixing
#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

// Thanks to the beautiful people at stack overflow
// https://stackoverflow.com/questions/55424746/is-there-an-analogous-function-to-vsnprintf-that-works-with-stdstring
int my_printf( const char * format, ... ) {
    // Buffer
    std::string result;
    va_list args, args_copy;

    // Open variadic macros
    va_start(args, format);
    va_copy(args_copy, args);

    // Do conversion from variadic to string
    const int len = vsnprintf(nullptr, 0, format, args);
    assert(len>0); // Sanity check, force only positives
    result.resize(len);
    vsnprintf(&result[0], len+1, format, args_copy);

    // Close variadic macros
    va_end(args_copy);
    va_end(args);

    // Enjoy the result
    UNSCOPED_INFO(std::move(result));
}
