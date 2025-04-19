#include <iostream>
#include <config.h>
#include <sstream>

#include "Vhd44780.h"
#include "verilated.h"

class test_hd44780 {
    public:
        test_hd44780(VerilatedContext& vc) : hd44780(Vhd44780{&vc}) {
            cycle = 0;
            hcycle = 0;
            le = 0;
            hd44780.rst = 0; // Negated so is now acting as reset
            hd44780.clk = 0;
            hd44780.trg = 0;
            // Evaluate to enforce at least one cycle for validity of outputs
            hd44780.eval();
        }

        std::string to_string() {
            std::stringstream ss;
            ss << "Cycle: " << cycle;
            ss << " Rst: " << (unsigned int)hd44780.rst;
            ss << " Clk: " << (unsigned int)hd44780.clk;
            ss << " Trg: " << (unsigned int)hd44780.trg;
            ss << " Busy: " << (unsigned int)hd44780.busy;
            ss << " E: " << (unsigned int)hd44780.e;
            ss << " RS: " << (unsigned int)hd44780.rs;
            ss << " DB: " << (unsigned int)hd44780.db;
            return ss.str();
        }

        void nextHalfCycle() {
            syncVariables();
            hd44780.clk = hd44780.clk ^ 0x01;
            cycle += hd44780.clk; // Only on full waveform
            hcycle++; // On each edge
            hd44780.eval();
        }

        void nextHCycle(const unsigned int cycles) {
            for(unsigned int i=0; i<cycles; i++) {
                nextHalfCycle();
            }
        }

        void nextCycle() {
            nextHCycle(2);
        }

        bool changeEnable() {
            return le ^ hd44780.e;
        }

        void setReset(unsigned char reset) {
            hd44780.rst = reset;
        }

        unsigned char busy() {
            return hd44780.busy;
        }

        unsigned long long getCycles() {
            return cycle;
        }

    private:
        Vhd44780 hd44780;
        unsigned long long cycle;
        unsigned long long hcycle;
        unsigned char le;

        void syncVariables() {
            this->le = hd44780.e;
        }
};

int main(int argc, char **argv, char **env)
{
    std::cout << "Program running: " << PACKAGE_STRING << std::endl;
    std::cout << "Program version: " << VERSION << std::endl;
    Verilated::commandArgs(argc, argv);
    unsigned long long int cnt = 0;
    unsigned long long int i = 0;
    const std::unique_ptr<VerilatedContext> contextp{new VerilatedContext};
    test_hd44780 hd(*contextp);
    std::cout << hd.to_string() << std::endl;
    for (int i=0; i<100; i++) {
        hd.nextHalfCycle();
        if (hd.changeEnable()) {
            std::cout << hd.to_string() << std::endl;
        }
    }
    hd.setReset(1);
    while(hd.busy()) {
        hd.nextHalfCycle();
        if (hd.changeEnable()) {
            std::cout << hd.to_string() << std::endl;
        }
    }
    std::cout << "Cycle count: " << hd.getCycles() << std::endl;
    exit(0);
}
