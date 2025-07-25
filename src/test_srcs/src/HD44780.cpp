#ifndef HD44780_HPP
#include "HD44780.hpp"
#endif
#ifndef HD44780GENERAL_HPP
#include "HD44780General.hpp"
#endif 

#include <sstream>
#include <bitset>

bool HD44780State::operator== (const HD44780State& o) const {
        return (this->rst == o.rst) &
        (this->clk == o.clk) &
        (this->trg == o.trg) &
        (this->busy == o.busy) &
        (this->e == o.e) &
        (this->rs == o.rs) &
        (this->db == o.db);
}

bool HD44780State::operator!= (const HD44780State& o) const {
    return !(*this == o);
}

// Check only the output values that are sent outside
// other flags are permitted to be changed
bool HD44780State::compareOutputs (const HD44780State& o) const {
        return (this->e == o.e) &
        (this->rs == o.rs) &
        (this->db == o.db);
}

std::string HD44780State::to_string() const {
    std::stringstream ss;
    ss << " Rst: " << (unsigned int)rst;
    ss << " Clk: " << (unsigned int)clk;
    ss << " Trg: " << (unsigned int)trg;
    ss << " Busy: " << (unsigned int)busy;
    ss << " E: " << (unsigned int)e;
    ss << " RS: " << (unsigned int)rs;
    ss << " DB (h): " << std::hex << (unsigned int)db;
    ss << " DB (b): " << std::bitset<4>((unsigned int)db);
    ss << " IData: " << std::bitset<8>((unsigned int)idata);
    ss << " IDataAddr: " << (unsigned int)idataaddr;
    ss << " IDataAddrRdy: " << (unsigned int)idataaddr_rdy;
    return ss.str();
}


// Class functions
WrapHD44780::WrapHD44780(VerilatedContext& vc) : hd44780(Vhd44780{&vc}) {
    cycle = 0;
    hcycle = 0;
    le = 0;
    hd44780.rst = 0; // Negated so is now acting as reset
    hd44780.clk = 0;
    hd44780.trg = 0;
}

std::string WrapHD44780::to_string() {
    std::stringstream ss;
    ss << "Cycle: " << cycle;
    ss << getState().to_string();
    return ss.str();
}

void WrapHD44780::nextHalfCycle() {
    syncVariables();
    hd44780.clk = hd44780.clk ^ 0x01;
    cycle += hd44780.clk; // Only on full waveform
    hcycle++; // On each edge
    hd44780.eval();
}

void WrapHD44780::nextNHalfCycles(const unsigned int cycles) {
    for(unsigned int i=0; i<cycles; i++) {
        nextHalfCycle();
    }
}

void WrapHD44780::nextCycle() {
    nextNHalfCycles(2);
}

bool WrapHD44780::changeEnable() {
    return (le ^ hd44780.e);
}

bool WrapHD44780::transitionEnableHigh() {
    return changeEnable() & hd44780.e;
}

void WrapHD44780::setReset(unsigned char reset) {
    hd44780.rst = reset;
}

unsigned long long WrapHD44780::getCycles() const {
    return cycle;
}

unsigned long long WrapHD44780::getHCycles() const {
    return hcycle;
}

void WrapHD44780::syncVariables() {
    this->le = hd44780.e;
}

HD44780State WrapHD44780::getState() const {
    return HD44780State {
        .clk = hd44780.clk,
        .rst = hd44780.rst,
        .trg = hd44780.trg,
        .busy = hd44780.busy,
        .busy_reset = hd44780.busy_reset,
        .busy_print = hd44780.busy_print,
        .e = hd44780.e,
        .rs = hd44780.rs,
        .db = hd44780.db,
        .idataaddr = hd44780.idataaddr,
        .idataaddr_rdy = hd44780.idataaddr_rdy,
        .idata = hd44780.idata,
    };
}

void WrapHD44780::setrst(unsigned char rst) {
    hd44780.rst = rst;
}

void WrapHD44780::settrg(unsigned char trg) {
    hd44780.trg = trg;
}

void WrapHD44780::setidata(unsigned char idata) {
    hd44780.idata = idata;
}
