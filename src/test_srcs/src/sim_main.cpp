#include <iostream>
#include <config.h>
#include <format>

#include "Vhd44780.h"
#include "verilated.h"

class test_hd44780 {
    public:
        test_hd44780(VerilatedContext& vc) : hd44780(Vhd44780{&vc}) {
            cycle = 0;
            hd44780.rst = 0;
            hd44780.clk = 0;
            hd44780.trg = 0;
            // Evaluate to enforce at least one cycle for validity of outputs
            hd44780.eval();
        }

        std::string to_string() {
            return std::format("Cycle: {} Rst: {} Clk: Trg: {} Busy: {} E: {} RS: {} DB: {}\n",
                cycle, (unsigned int)hd44780.rst, (unsigned int)hd44780.clk,
                (unsigned int)hd44780.trg, (unsigned int)hd44780.busy,
                (unsigned int)hd44780.e, (unsigned int)hd44780.rs, 
                (unsigned int)hd44780.db
            );
        }

    private:
        Vhd44780 hd44780;
        unsigned long long cycle;

        void nextHalfCycle() {
            hd44780.clk = hd44780.clk ^ 0x01;
            hd44780.eval();
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
    const std::unique_ptr<Vhd44780> top{new Vhd44780{contextp.get()}};
    test_hd44780 hd(*contextp);
    std::cout << hd.to_string() << std::endl;
    contextp->internalsDump();  // See scopes to help debug
    top->rst = 0;
    top->clk = 0;
    top->trg = 0;
    unsigned char le = 0;
    for(i=0; i<100; i++) {
        if (top->e != le) {
            std::cout << "ICycle: " << i;
            std::cout << " Rst: " << (unsigned int)top->rst << " Clk: " << (unsigned int)top->clk << " Trg " << (unsigned int)top->trg;
            std::cout << " Bsy: " << (unsigned int)top->busy << " E: " << (unsigned int)top->e << " RS: " << (unsigned int)top->rs << " DB: " << (unsigned int)top->db << std::endl;
        }
        le = top->e;
        top->eval();
        top->clk = top->clk ^ 0x01;
    }
    top->rst = 1;
    while(top->busy) {
        if (top->e != le) {
            std::cout << "SCycle: " << i;
            std::cout << " Rst: " << (unsigned int)top->rst << " Clk: " << (unsigned int)top->clk << " Trg " << (unsigned int)top->trg;
            std::cout << " Bsy: " << (unsigned int)top->busy << " E: " << (unsigned int)top->e << " RS: " << (unsigned int)top->rs << " DB: " << (unsigned int)top->db << std::endl;
        }
        le = top->e;
        top->eval();
        top->clk = top->clk ^ 0x01;
        i++;
    }
    std::cout << "Cycle count: " << cnt << std::endl;
    exit(0);
}
