#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include "config.h"

// Definitions for the system in question to be tested
#include "verilated.h"
#ifndef HD44780_HPP
#include "HD44780.hpp"
#endif
#ifndef HD44780GENERAL_HPP
#include "HD44780General.hpp"
#endif 

void resetSequence(WrapHD44780 &hd) {
    hd.setrst(0);
    hd.settrg(0);
    hd.nextHalfCycle();
    HD44780State old = hd.getState();
    // Due to doing each edge, 100 cycles
    for (int i=0; i<200; i++) {
        hd.nextCycle();
        HD44780State cur = hd.getState();
        unsigned char comp = old.compareOutputs(cur);
        if (!comp) {
            std::cout << "Initial: " << old.to_string() << std::endl;
            std::cout << "Initial: " << cur.to_string() << std::endl;
        }
        REQUIRE(comp);
    }
}

TEST_CASE("Reset of HD44780") {
    const std::unique_ptr<VerilatedContext> contextp{new VerilatedContext};
    WrapHD44780 hd(*contextp);
    resetSequence(hd);
}

TEST_CASE("Initialization of HD44780") {
    const std::unique_ptr<VerilatedContext> contextp{new VerilatedContext};
    WrapHD44780 hd(*contextp);
    resetSequence(hd);
}

int main( int argc, char* argv[] ) {
    // The version string MUST be always the first line to be printed
    std::cout << "Commit string: " << VERSION << std::endl;
    // Verilated commandArgs is a global macro/variables type definitions
    Verilated::commandArgs(argc, argv);
    // Entry point for all the library
    int result = Catch::Session().run( argc, argv );
    return result;
}
