#include <iostream>
#include <config.h>

#include "Vhd44780.h"
#include "verilated.h"
#ifndef HD44780_HPP
#include "hd44780.hpp"
#endif
#ifndef HD44780GENERAL_HPP
#include "hd44780general.hpp"
#endif 


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

    std::cout << "Printout of the known good reset sequence" << std::endl;
    reset_sequence();
    exit(0);
}
