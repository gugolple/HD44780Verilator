#include <iostream>

#include "Vhd44780.h"
#include "verilated.h"
int main(int argc, char **argv, char **env)
{
        Verilated::commandArgs(argc, argv);
        unsigned long long int cnt = 0;
        Vhd44780 *top = new Vhd44780;
        top->rst = 0;
        for(int i=0; i<10; i++){
                top->clk ^= 1;
                cnt += 1;
                top->eval();
        }
        top->rst = 1;
        std::cout << "Wait initialization clk:" << cnt << std::endl;
        while (!top->busy)
        {
                top->clk ^= 1;
                cnt += 1;
                top->eval();
        }
        delete top;
        std::cout << "Cycle count: " << cnt << std::endl;
        exit(0);
}
