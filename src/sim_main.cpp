#include <iostream>

#include "Vhd44780.h"
#include "verilated.h"
int main(int argc, char **argv, char **env)
{
        Verilated::commandArgs(argc, argv);
        Vhd44780 *top = new Vhd44780;
        while (!top->rdy)
        {
                if (top->clock)
                        std::cout << " " << top->e << std::endl;
                top->clock ^= 1;
                top->eval();
        }
        delete top;
        exit(0);
}
