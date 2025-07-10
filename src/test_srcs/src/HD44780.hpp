#ifndef HD44780_HPP
#define HD44780_HPP
#include "Vhd447804b.h"
#include "verilated.h"

typedef Vhd447804b Vhd44780;

struct HD44780State {
    unsigned char clk;
    unsigned char rst;
    unsigned char trg;
    unsigned char busy;
    unsigned char busy_reset;
    unsigned char busy_print;
    unsigned char e;
    unsigned char rs;
    unsigned char db;
    unsigned char idataaddr;
    unsigned char idataaddr_rdy;
    unsigned char idata;

    bool operator== (const HD44780State& o) const;
    bool operator!= (const HD44780State& o) const;
    bool compareOutputs(const HD44780State& o) const;
    std::string to_string() const; 
};

class WrapHD44780 {
    public:
        WrapHD44780(VerilatedContext& vc);
        std::string to_string(); 
        void nextHalfCycle();
        void nextNHalfCycles(const unsigned int cycles);
        void nextCycle();
        bool changeEnable();
        bool transitionEnableHigh();
        void setReset(unsigned char reset);
        unsigned long long getCycles() const;
        unsigned long long getHCycles() const;

        // Interface with the system below
        HD44780State getState() const;

        void setrst(unsigned char);
        void settrg(unsigned char);
        void setidata(unsigned char);

    private:
        Vhd44780 hd44780;
        unsigned long long cycle;
        unsigned long long hcycle;
        unsigned char le;

        void syncVariables();
};
#endif
