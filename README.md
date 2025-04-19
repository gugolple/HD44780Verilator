# verilator-hd44780

This project is to test and verify the correct behaviour of my implementation of
an HD44780 controller on verilog. Test based on verilator.


## Commands

The following list of key targets through the top level makefile are provided:

- ```make container-exec-run```
    - Provides the default full compilation and execution steps
- ```make container-it```
    - Allows the user to enter in the container environment, useful for debugging
    either the software with gdb or other tasks.

Within the container the commands are expected to be run with: **make exec-%**
where % is the command to be forwarded to the **Makefile** in the **src** folder.

All commands available in the **src/Makefile** are available through **make container-exec-%**
where **%** stands for the desired command.

# References
Based on [tommythorn/verilator-demo](https://github.com/tommythorn/verilator-demo)
