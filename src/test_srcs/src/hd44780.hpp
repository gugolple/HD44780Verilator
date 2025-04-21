#define HD44780_HPP
#include "Vhd44780.h"
#include "verilated.h"
class test_hd44780 {
    public:
        test_hd44780(VerilatedContext& vc);
        std::string to_string(); 
        void nextHalfCycle();
        void nextNHalfCycles(const unsigned int cycles);
        void nextCycle();
        bool changeEnable();
        bool transitionEnableHigh();
        void setReset(unsigned char reset);
        unsigned long long getCycles();
        unsigned char getrst();
        unsigned char getclk();
        unsigned char gettrg();
        unsigned char getbusy();
        unsigned char gete();
        unsigned char getrs();
        unsigned char getdb();

    private:
        Vhd44780 hd44780;
        unsigned long long cycle;
        unsigned long long hcycle;
        unsigned char le;

        void syncVariables();
};
