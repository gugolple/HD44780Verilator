#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include "config.h"
#include <cassert>
#include <algorithm>

// Definitions for the system in question to be tested
#include "verilated.h"
#ifndef HD44780_HPP
#include "HD44780.hpp"
#endif
#ifndef HD44780GENERAL_HPP
#include "HD44780General.hpp"
#endif 

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

#define MAX_COMMAND_WAIT_SENT convertSecondsToHalfCycleCount(1)

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

#define infoCommand(hd) INFO( "Command sent at cycle: " << hd.getCycles() << "\n" "- " << hd.getState().to_string() << "\n")

void failedCompareModelAndSimulation(bool b, HD44780State const &hd, 
    HD44780Payload const &pld, unsigned char payloadval) {
    FAIL("Model and simulation differ"
        << "\n- Model RS: " << (unsigned int)pld.getrs() << " DB: " 
            << (unsigned int)payloadval
        << "\n- Simul RS: " << (unsigned int)hd.rs << " DB: " 
            << (unsigned int)hd.db
    );
}

bool compareModelAndSimulation(HD44780State const &hd, HD44780Payload const &pld,
    unsigned char payloadval) {
    bool res = (hd.rs == pld.getrs()) &
        (hd.db == payloadval);
    if (!res) {
        failedCompareModelAndSimulation(res, hd, pld, payloadval);
    }
    return res;
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

void printState(WrapHD44780 const &hd, HD44780State const &initial, HD44780State const &cur
        , unsigned long long int tgt) {
    FAIL( "Failure to maintain " << "at cycle: " << hd.getCycles() 
            << " at hcycle: " << hd.getHCycles() <<  " htarget: " << tgt << "\n"
            << "- Initial: " << initial.to_string() << "\n"
            << "- Current: " << cur.to_string() << "\n" );
}

void maintainStateHalfs(WrapHD44780 &hd, const int hcycles, HD44780State &initial) {
    const unsigned long long htgt = hd.getHCycles() + hcycles;
    for (int i=0; i<hcycles; i++) {
        hd.nextHalfCycle();
        HD44780State cur = hd.getState();
        unsigned char comp = initial.compareOutputs(cur);
        if (!comp) {
            printState(hd, initial, cur, htgt);
        }
        REQUIRE(comp);
    }
}

void maintainStateCycles(WrapHD44780 &hd, const int cycles, HD44780State &initial) {
    const unsigned long long tgt = hd.getCycles() + cycles;
    for (int i=0; i<cycles; i++) {
        hd.nextCycle();
        HD44780State cur = hd.getState();
        unsigned char comp = initial.compareOutputs(cur);
        if (!comp) {
            printState(hd, initial, cur, tgt);
        }
        REQUIRE(comp);
    }
}

void timeoutError(WrapHD44780 const &hd, const unsigned long long int initial) {
    FAIL("Wait for command too long.\n- Initial command cycle: "
        << initial << "\n- Current command cycle: " << hd.getHCycles()
    );
}

void waitUntilCommandSent(WrapHD44780 &hd) {
    const unsigned long long int initialHCycle = hd.getHCycles();
    const unsigned long long int targetHCycle = initialHCycle + MAX_COMMAND_WAIT_SENT;
    // E pin known to be high with the loop
    while (!hd.getState().e) {
        hd.nextHalfCycle();
        if(hd.getHCycles() >= targetHCycle) {
            timeoutError(hd, initialHCycle);
        }
    };
    INFO( "E is high at " << hd.getCycles() << "\n" );
    while (hd.getState().e) {
        hd.nextHalfCycle();
        if(hd.getHCycles() >= targetHCycle) {
            timeoutError(hd, initialHCycle);
        }
    };
    INFO( "E is low at " << hd.getCycles() << "\n" );
}

void waitUntilChange(WrapHD44780 &hd, HD44780State &initial) {
    HD44780State cur = hd.getState();
    while(initial.compareOutputs(cur)) {
        hd.nextHalfCycle();
        cur = hd.getState();
    }
}

void receiveSingleData(WrapHD44780 &hd, const char *l, unsigned int pos,
        const unsigned int offset) {
    // Always give the requested value
    const unsigned long long int initialHCycle = hd.getHCycles();
    const unsigned long long int targetHCycle = initialHCycle + MAX_COMMAND_WAIT_SENT;
    // E pin known to be high with the loop
    REQUIRE(!hd.getState().e);
    while (!hd.getState().e) {
        // Every clock provide requested idata from idataaaddr
        hd.nextHalfCycle();
        if(hd.getHCycles() >= targetHCycle) {
            timeoutError(hd, initialHCycle);
        }
    };
    INFO( "E is high at " << hd.getCycles() << "\n" );
    REQUIRE(((unsigned int)hd.getState().idataaddr - offset) == pos);
    hd.setidata(l[hd.getState().idataaddr - offset]);
    // E and RS have to be high
    REQUIRE((hd.getState().e && hd.getState().rs));
    while (hd.getState().e) {
        hd.nextHalfCycle();
        if(hd.getHCycles() >= targetHCycle) {
            timeoutError(hd, initialHCycle);
        }
    };
    INFO( "E is low at " << hd.getCycles() << "\n" );
    REQUIRE((!hd.getState().e && hd.getState().rs));
}

// Offset for when lines L2 and L4 that start reading the buffer at 32
void receiveData(WrapHD44780 &hd, const char *l, const unsigned int offset) {
    for(int i=0; i<(ROWLENACT*2); i++) {
        INFO("Character of sequence: " << l[i] << " number: " << i);
        receiveSingleData(hd, l, i, offset);
        const unsigned char high_bits = hd.getState().db; 
        receiveSingleData(hd, l, i, offset);
        const unsigned char low_bits = hd.getState().db; 
        //const unsigned char actual_received = high_bits << 4 | low_bits;
        const unsigned char actual_received = high_bits << 4 | low_bits;
        REQUIRE((unsigned int)actual_received == (unsigned int)l[i]);
    }
}

void checkReceptionOfLines(WrapHD44780 &hd, const char *l1, const char *l2,
        const unsigned int offset) {
    char combinedLines[ROWLENACT*2+1];
    for(int i=0; i<ROWLENACT; i++) {
        combinedLines[i] = l1[i];
    }
    for(int i=0; i<ROWLENACT; i++) {
        combinedLines[i+ROWLENACT] = l2[i];
    }
    // Force last value to be 0 to comply with C strings
    combinedLines[ROWLENACT*2] = '\0';
    receiveData(hd, combinedLines, offset);
}

void checkFullCommandSent(WrapHD44780 &hd, HD44780Payload const &pld) {
    waitUntilCommandSent(hd);
    infoCommand(hd);
    REQUIRE(compareModelAndSimulationHigh(hd.getState(), pld));
    waitUntilCommandSent(hd);
    infoCommand(hd);
    REQUIRE(compareModelAndSimulationLow(hd.getState(), pld));
}

void initialReset(WrapHD44780 &hd) {
    hd.setrst(0);
    hd.settrg(0);
    hd.nextHalfCycle();
    HD44780State old = hd.getState();
    // Arbitrary value, just an amount greater than 1 cycle to give the system
    // an opportunity
    INFO("Commit reset");
    maintainStateCycles(hd, 100, old);
    hd.setrst(1);
}

void resetSequence(WrapHD44780 &hd) {
    initialReset(hd);
    HD44780State old = hd.getState();

    // Wait 100ms for restart
    INFO("Restart wait");
    old = hd.getState();
    maintainStateHalfs(hd, convertMilliSecondsToHalfCycleCount(HD44780_POWERON_DELAY_MS), old);

    // Instruction function set for half, first time
    INFO("Function set 1");
    waitUntilCommandSent(hd);
    REQUIRE(compareModelAndSimulationHigh(hd.getState(), hd44780_inst_function_set_half()));

    // Wait 10ms for a clear display 
    INFO("Function set 1 wait");
    old = hd.getState();
    maintainStateHalfs(hd, convertMilliSecondsToHalfCycleCount(HD44780_INST_CLEAR_DISPLAY_MS), old);

    // Read full function set for half, lines and font
    INFO("Function set 2 complete");
    checkFullCommandSent(hd,  hd44780_inst_function_set());

    // Wait 10ms for a clear display 
    INFO("Function set 2 wait");
    old = hd.getState();
    maintainStateHalfs(hd, convertMilliSecondsToHalfCycleCount(HD44780_INST_CLEAR_DISPLAY_MS), old);

    // Execute clear display command
    INFO("Clear display");
    checkFullCommandSent(hd,  hd44780_inst_display_clear());

    // Wait 10ms for a clear display 
    INFO("Clear display wait");
    old = hd.getState();
    maintainStateHalfs(hd, convertMilliSecondsToHalfCycleCount(HD44780_INST_CLEAR_DISPLAY_MS), old);

    // Execute display control command
    INFO("Display control");
    checkFullCommandSent(hd,  hd44780_inst_display_control(
        HD44780_CONFIG_D_DISPLAY_ONOFF,
        HD44780_CONFIG_C_CURSOR_ONOFF,
        HD44780_CONFIG_C_CURSOR_BLINK
    ));

    // Wait for instruction processed
    INFO("Display control wait");
    old = hd.getState();
    maintainStateHalfs(hd, convertMicroSecondsToCycleCount(HD44780_INST_DELAY_US), old);

    // Execute entry mode set command
    INFO("Entry mode set");
    checkFullCommandSent(hd, hd44780_inst_entry_mode_set(
        HD44780_CONFIG_ID_INCREMENT_DIRECTION,
        HD44780_CONFIG_SHIFT_CURSOR
    ));

    // Wait for instruction processed
    INFO("Entry mode wait");
    old = hd.getState();
    maintainStateHalfs(hd, convertMicroSecondsToHalfCycleCount(HD44780_INST_DELAY_US), old);

    // Confirm that at some point later the busy_reset flag is set to 0
    // indicating the process has been completed
    INFO("Wait until busy_reset is low.");
    const unsigned long long int initialHCycle = hd.getHCycles();
    const unsigned long long int targetHCycle = initialHCycle + MAX_COMMAND_WAIT_SENT;
    while(hd.getState().busy_reset) {
        hd.nextHalfCycle();
        if(hd.getState().e) {
            FAIL("Command received after full reset sequence" 
                << (unsigned int)hd.getState().rs << " DB: " 
                << (unsigned int)hd.getState().db
            );
        }
        if(hd.getHCycles() >= targetHCycle) {
            timeoutError(hd, initialHCycle);
        }
    }
    // Add positive check in addition to the error that will trigger in case of
    // failure before
    REQUIRE(1);
}

TEST_CASE("Full Reset of HD44780", "[RST]") {
    const std::unique_ptr<VerilatedContext> contextp{new VerilatedContext};
    WrapHD44780 hd(*contextp);
    resetSequence(hd);
}

TEST_CASE("Initialization of HD44780", "[TRS]") {
    const std::unique_ptr<VerilatedContext> contextp{new VerilatedContext};
    WrapHD44780 hd(*contextp);

    // Printing sequence
    // This flag indicates the state machine of the reset sequence has been
    // compleated
    INFO("Realize initial reset sequence.");
    initialReset(hd);
    INFO("Wait until busy_reset is low.");
    const unsigned long long int initialHCycle = hd.getHCycles();
    const unsigned long long int targetHCycle = initialHCycle + MAX_COMMAND_WAIT_SENT;
    while(hd.getState().busy_reset) {
        hd.nextHalfCycle();
        if(hd.getHCycles() >= targetHCycle) {
            timeoutError(hd, initialHCycle);
        }
    }
    // Add positive check in addition to the error that will trigger in case of
    // failure before
    REQUIRE(1);

    // Check the receptions of L1 and L3
    INFO("Receive set DDRAM address to L1");
    checkFullCommandSent(hd, hd44780_inst_set_ddram_l1());

    INFO("Receive L1 and L3");
    checkReceptionOfLines(hd, "1234567890abcdfe", "efcdba0987654321", 0);

    INFO("Receive set DDRAM address to L2");
    checkFullCommandSent(hd, hd44780_inst_set_ddram_l2());

    INFO("Receive L2 and L4");
    checkReceptionOfLines(hd, "1234567890ABCDFE", "EFCDBA0987654321", ROWLENACT*2);
}

int main( int argc, char* argv[] ) {
    // The version string MUST be always the first line to be printed
    std::cout << "===============================================================================\n";
    std::cout << "Test suite commencing\n";
    std::cout << "Srcs commit: " << SRCSVERSION << "\n";
    std::cout << "Test suite commit: " << VERSION << "\n";
    std::cout << "===============================================================================\n";
    // Verilated commandArgs is a global macro/variables type definitions
    Verilated::commandArgs(argc, argv);
    // Entry point for all the library
    int result = Catch::Session().run( argc, argv );
    return result;
}
