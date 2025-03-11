#!/bin/bash
verilator -Wall --cc hd44780.v --exe sim_main.cpp
make -C obj_dir -j -f Vhd44780.mk Vhd44780
