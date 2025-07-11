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
#include <iostream>

// Catch2 to do the mixing
#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

// Thanks to the beautiful people at stack overflow
// https://stackoverflow.com/questions/55424746/is-there-an-analogous-function-to-vsnprintf-that-works-with-stdstring
// https://stackoverflow.com/questions/56331128/call-to-snprintf-and-vsnprintf-inside-a-variadic-function
int my_printf( const char * format, ... ) {
    // Buffer
    std::string result;
    va_list args, args_copy;

    // Open variadic macros
    va_start(args, format);
    va_copy(args_copy, args);

    size_t size = vsnprintf(nullptr, 0, format, args) + 1; 
    assert(size>0); // Sanity check, force only positives
    va_end(args_copy);

    // Format the string
    char *szBuff = new char[size];
    vsnprintf(szBuff, size, format, args);
    va_end(args);


    std::cout << szBuff << std::endl;

    // Enjoy the result
    //UNSCOPED_INFO(szBuff);
    delete[] szBuff;
}
