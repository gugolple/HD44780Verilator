#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include "config.h"
#include <cassert>

// Definitions for the system in question to be tested
#include "verilated.h"
#ifndef HD44780_HPP
#include "HD44780.hpp"
#endif
#ifndef HD44780GENERAL_HPP
#include "HD44780General.hpp"
#endif 

// Defines the temporal basis for all calculations in how many units of a second.
// This factor should be applied to any and all second calculations.
// The idea is to keep everything as a positive integer without floating point
//     calculations.
#define MS 1'000
#define US 1'000'000
#define NS 1'000'000'000
#define TIME_BASE NS
#define FHZ 250'000
#define HFHZ 2*FHZ

#define PERIOD_FHZ getPeriodOnBase(FHZ)
#define PERIOD_HFHZ getPeriodOnBase(HFHZ)

constexpr unsigned long long getPeriodOnBase(unsigned long long freq) {
    return TIME_BASE/freq;
}

constexpr unsigned long long convertSecondsToCycleCount(unsigned long long time) {
    return time * FHZ;
}

constexpr unsigned long long convertMilliSecondsToCycleCount(unsigned long long time) {
    unsigned long long val = time * FHZ / MS;
    assert(val);
    return val;
}

constexpr unsigned long long convertMicroSecondsToCycleCount(unsigned long long time) {
    unsigned long long val = time * FHZ / US;
    assert(val);
    return val;
}

constexpr unsigned long long convertNanoSecondsToCycleCount(unsigned long long time) {
    unsigned long long val = time * FHZ / NS;
    assert(val);
    return val;
}

void maintainState(WrapHD44780 &hd, const int cycles, HD44780State &initial) {
    for (int i=0; i<200; i++) {
        hd.nextCycle();
        HD44780State cur = hd.getState();
        unsigned char comp = initial.compareOutputs(cur);
        if (!comp) {
            std::cout << "Initial: " << initial.to_string() << std::endl;
            std::cout << "Initial: " << cur.to_string() << std::endl;
        }
        REQUIRE(comp);
    }
}

void resetSequence(WrapHD44780 &hd) {
    hd.setrst(0);
    hd.settrg(0);
    hd.nextHalfCycle();
    HD44780State old = hd.getState();
    // Due to doing each edge, 100 cycles
    maintainState(hd, 100*2, old);
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

    // Wait 100ms for restart
    HD44780State old = hd.getState();
}

int main( int argc, char* argv[] ) {
    // The version string MUST be always the first line to be printed
    std::cout << "Commit string: " << VERSION << std::endl;
    // Verilated commandArgs is a global macro/variables type definitions
    Verilated::commandArgs(argc, argv);
    // Entry point for all the library
    int result = Catch::Session().run( argc, argv );
    std::cout << convertSecondsToCycleCount(100) << std::endl;
    std::cout << convertMilliSecondsToCycleCount(100) << std::endl;
    std::cout << convertMicroSecondsToCycleCount(100) << std::endl;
    std::cout << convertNanoSecondsToCycleCount(100) << std::endl;
    return result;
}
