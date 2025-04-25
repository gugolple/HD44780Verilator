#ifndef HD44780_HPP
#define HD44780_HPP
#include "Vhd44780.h"
#include "verilated.h"

struct HD44780State {
    unsigned char rst;
    unsigned char clk;
    unsigned char trg;
    unsigned char busy;
    unsigned char e;
    unsigned char rs;
    unsigned char db;

    bool operator== (const HD44780State& o);
    bool operator!= (const HD44780State& o);
    bool compareOutputs(const HD44780State& o);
    std::string to_string(); 
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
        unsigned long long getCycles();

        // Interface with the system below
        unsigned char getrst();
        unsigned char getclk();
        unsigned char gettrg();
        unsigned char getbusy();
        unsigned char gete();
        unsigned char getrs();
        unsigned char getdb();
        HD44780State getState();

        void setrst(unsigned char);
        void settrg(unsigned char);

    private:
        Vhd44780 hd44780;
        unsigned long long cycle;
        unsigned long long hcycle;
        unsigned char le;

        void syncVariables();
};
#endif
