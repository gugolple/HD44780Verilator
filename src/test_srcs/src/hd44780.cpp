#ifndef hd44780_HPP
#include "hd44780.hpp"
#endif
#ifndef HD44780GENERAL_HPP
#include "hd44780general.hpp"
#endif 

#include <sstream>
#include <bitset>


// Class functions
test_hd44780::test_hd44780(VerilatedContext& vc) : hd44780(Vhd44780{&vc}) {
    cycle = 0;
    hcycle = 0;
    le = 0;
    hd44780.rst = 0; // Negated so is now acting as reset
    hd44780.clk = 0;
    hd44780.trg = 0;
    // Evaluate to enforce at least one cycle for validity of outputs
    hd44780.eval();
}

std::string test_hd44780::to_string() {
    std::stringstream ss;
    ss << "Cycle: " << cycle;
    ss << " Rst: " << (unsigned int)hd44780.rst;
    ss << " Clk: " << (unsigned int)hd44780.clk;
    ss << " Trg: " << (unsigned int)hd44780.trg;
    ss << " Busy: " << (unsigned int)hd44780.busy;
    ss << " E: " << (unsigned int)hd44780.e;
    ss << " RS: " << (unsigned int)hd44780.rs;
    ss << " DB (h): " << std::hex << (unsigned int)hd44780.db;
    ss << " DB (b): " << std::bitset<4>((unsigned int)hd44780.db);
    return ss.str();
}

void test_hd44780::nextHalfCycle() {
    syncVariables();
    hd44780.clk = hd44780.clk ^ 0x01;
    cycle += hd44780.clk; // Only on full waveform
    hcycle++; // On each edge
    hd44780.eval();
}

void test_hd44780::nextNHalfCycles(const unsigned int cycles) {
    for(unsigned int i=0; i<cycles; i++) {
        nextHalfCycle();
    }
}

void test_hd44780::nextCycle() {
    nextNHalfCycles(2);
}

bool test_hd44780::changeEnable() {
    return (le ^ hd44780.e);
}

bool test_hd44780::transitionEnableHigh() {
    return changeEnable() & hd44780.e;
}

void test_hd44780::setReset(unsigned char reset) {
    hd44780.rst = reset;
}

unsigned long long test_hd44780::getCycles() {
    return cycle;
}

unsigned char test_hd44780::getrst() {
    return hd44780.rst;
}

unsigned char test_hd44780::getclk() {
    return hd44780.clk;
}

unsigned char test_hd44780::gettrg() {
    return hd44780.trg;
}

unsigned char test_hd44780::getbusy() {
    return hd44780.busy;
}

unsigned char test_hd44780::gete() {
    return hd44780.e;
}

unsigned char test_hd44780::getrs() {
    return hd44780.rs;
}

unsigned char test_hd44780::getdb() {
    return hd44780.db;
}

void test_hd44780::syncVariables() {
    this->le = hd44780.e;
}
