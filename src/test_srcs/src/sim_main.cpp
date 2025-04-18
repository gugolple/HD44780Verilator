#include <iostream>
#include <config.h>

#include "Vhd44780.h"
#include "verilated.h"

int main(int argc, char **argv, char **env)
{
        std::cout << "Program running: " << PACKAGE_STRING << std::endl;
        std::cout << "Program version: " << VERSION << std::endl;
        Verilated::commandArgs(argc, argv);
        unsigned long long int cnt = 0;
        unsigned long long int i = 0;
        const std::unique_ptr<VerilatedContext> contextp{new VerilatedContext};
        const std::unique_ptr<Vhd44780> top{new Vhd44780{contextp.get()}};
        contextp->internalsDump();  // See scopes to help debug
        top->rst = 0;
        top->clk = 0;
        top->trg = 0;
        for(i=0; i<100; i++) {
            if (top->e) {
                std::cout << "ICycle: " << i;
                std::cout << " Rst: " << (unsigned int)top->rst << " Clk: " << (unsigned int)top->clk << " Trg " << (unsigned int)top->trg;
                std::cout << " Bsy: " << (unsigned int)top->busy << " E: " << (unsigned int)top->e << " RS: " << (unsigned int)top->rs << " DB: " << (unsigned int)top->db << std::endl;
            }
            top->eval();
            top->clk = top->clk ^ 0x01;
        }
        top->rst = 1;
        while(top->busy) {
            if (top->e) {
                std::cout << "SCycle: " << i;
                std::cout << " Rst: " << (unsigned int)top->rst << " Clk: " << (unsigned int)top->clk << " Trg " << (unsigned int)top->trg;
                std::cout << " Bsy: " << (unsigned int)top->busy << " E: " << (unsigned int)top->e << " RS: " << (unsigned int)top->rs << " DB: " << (unsigned int)top->db << std::endl;
            }
            top->eval();
            top->clk = top->clk ^ 0x01;
            i++;
        }
        std::cout << "Cycle count: " << cnt << std::endl;
        exit(0);
}
