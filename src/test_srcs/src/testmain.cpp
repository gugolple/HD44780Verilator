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

bool compareModelAndSimulation(HD44780State const &hd, HD44780Payload const &pld, unsigned char payloadval) {
    return (hd.rs == pld.getrs()) &
        (hd.db == payloadval);
}

bool compareModelAndSimulationHigh(HD44780State const &hd, HD44780Payload const &pld) {
    return compareModelAndSimulation(hd, pld, pld.gethighbits());
}

bool compareModelAndSimulationLow(HD44780State const &hd, HD44780Payload const &pld) {
    return compareModelAndSimulation(hd, pld, pld.getlowbits());
}

constexpr unsigned long long getPeriodOnBase(unsigned long long freq) {
    return TIME_BASE/freq;
}

constexpr unsigned long long convertSecondsToCycleCount(unsigned long long time) {
    return time * FHZ;
}

constexpr unsigned long long convertMilliSecondsToCycleCount(unsigned long long time) {
    unsigned long long val = convertSecondsToCycleCount(time) / MS;
    assert(val);
    return val;
}

constexpr unsigned long long convertMicroSecondsToCycleCount(unsigned long long time) {
    unsigned long long val = convertSecondsToCycleCount(time) / US;
    assert(val);
    return val;
}

constexpr unsigned long long convertNanoSecondsToCycleCount(unsigned long long time) {
    unsigned long long val = convertSecondsToCycleCount(time) / NS;
    assert(val);
    return val;
}

constexpr unsigned long long convertSecondsToHalfCycleCount(unsigned long long time) {
    return time * HFHZ;
}

constexpr unsigned long long convertMilliSecondsToHalfCycleCount(unsigned long long time) {
    unsigned long long val = convertSecondsToHalfCycleCount(time) / MS;
    assert(val);
    return val;
}

constexpr unsigned long long convertMicroSecondsToHalfCycleCount(unsigned long long time) {
    unsigned long long val = convertSecondsToHalfCycleCount(time) / US;
    assert(val);
    return val;
}

constexpr unsigned long long convertNanoSecondsToHalfCycleCount(unsigned long long time) {
    unsigned long long val = convertSecondsToHalfCycleCount(time) / NS;
    assert(val);
    return val;
}

void printState(WrapHD44780 const &hd, HD44780State const &initial, HD44780State const &cur) {
     FAIL( "Failure to maintain " << "at cycle: " << hd.getCycles() 
     << " at hcycle: " << hd.getHCycles() << "\n"
     << "Initial: " << initial.to_string() << "\n"
     << "Current: " << cur.to_string() << "\n" );
}

void maintainStateHalfs(WrapHD44780 &hd, const int hcycles, HD44780State &initial) {
    for (int i=0; i<hcycles; i++) {
        hd.nextHalfCycle();
        HD44780State cur = hd.getState();
        unsigned char comp = initial.compareOutputs(cur);
        if (!comp) {
            printState(hd, initial, cur);
        }
        REQUIRE(comp);
    }
}

void maintainStateCycles(WrapHD44780 &hd, const int cycles, HD44780State &initial) {
    for (int i=0; i<cycles; i++) {
        hd.nextCycle();
        HD44780State cur = hd.getState();
        unsigned char comp = initial.compareOutputs(cur);
        if (!comp) {
            printState(hd, initial, cur);
        }
        REQUIRE(comp);
    }
}

void waitUntilCommandSent(WrapHD44780 &hd) {
    // E pin known to be high with the loop
    while (!hd.gete()) {hd.nextHalfCycle();};
    INFO( "E is high at " << hd.getCycles() << "\n" );
    while (hd.gete()) {hd.nextHalfCycle();};
    INFO( "E is low at " << hd.getCycles() << "\n" );
}

void waitUntilChange(WrapHD44780 &hd, HD44780State &initial) {
    HD44780State cur = hd.getState();
    while(initial.compareOutputs(cur)) {
        hd.nextHalfCycle();
        cur = hd.getState();
    }
}

void resetSequence(WrapHD44780 &hd) {
    hd.setrst(0);
    hd.settrg(0);
    hd.nextHalfCycle();
    HD44780State old = hd.getState();
    // Arbitrary value, just an amount greater than 1 cycle to give the system
    // an opportunity
    maintainStateCycles(hd, 100, old);
    hd.setrst(1);
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
    maintainStateHalfs(hd, convertMilliSecondsToHalfCycleCount(100), old);

    // Instruction function set for half, first time
    waitUntilCommandSent(hd);
    INFO( "Command sent at: " << hd.getCycles() << " " << hd.getState().to_string() << "\n");
    REQUIRE(compareModelAndSimulationHigh(hd.getState(), hd44780_inst_function_set_half()));

    // Wait 10ms for a clear display 
    old = hd.getState();
    maintainStateHalfs(hd, convertMilliSecondsToHalfCycleCount(10), old);
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
