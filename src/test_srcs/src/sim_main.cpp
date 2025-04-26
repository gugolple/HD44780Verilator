#include <iostream>
#include <config.h>

#include "Vhd44780.h"
#include "verilated.h"
#ifndef HD44780_HPP
#include "HD44780.hpp"
#endif
#ifndef HD44780GENERAL_HPP
#include "HD44780General.hpp"
#endif 


int main(int argc, char **argv, char **env)
{
    // Standard printout to see current status
    std::cout << "Program running: " << PACKAGE_STRING << std::endl;
    std::cout << "Commit string: " << VERSION << std::endl;

    // Verilator specifics
    Verilated::commandArgs(argc, argv);
    const std::unique_ptr<VerilatedContext> contextp{new VerilatedContext};
    WrapHD44780 hd(*contextp);

    // Tests
    unsigned long long int cnt = 0;
    unsigned long long int i = 0;
    std::cout << hd.to_string() << std::endl;
    for (int i=0; i<100; i++) {
        hd.nextHalfCycle();
        if (hd.transitionEnableHigh()) {
            std::cout << hd.to_string() << std::endl;
        }
    }
    hd.setReset(1); // Reset is active low on the FPGA model
    while(hd.getbusy()) {
        hd.nextHalfCycle();
        if (hd.transitionEnableHigh()) {
            std::cout << hd.to_string() << std::endl;
        }
    }
    std::cout << "Cycle count: " << hd.getCycles() << std::endl;
    exit(0);
}
