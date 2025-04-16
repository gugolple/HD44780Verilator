.PHONY: run

VERILATOR_EXTRA_OPTS = --top-module hd44780 -Wno-INITIALDLY -Wno-UNUSED -Wno-UNDRIVEN 
PATH_SRC = fpga_gowin_hd44780/src
VERILOG_SRCS = $(wildcard ${PATH_SRC}/*.v)
MEMS = $(wildcard ${PATH_SRC}/*.mem)
MEMS_LOC = $(notdir ${MEMS})

%.mem: ${PATH_SRC}/%.mem 
	cp $< $@

all: obj_dir/Vhd44780

run: obj_dir/Vhd44780
	obj_dir/Vhd44780

test: run

obj_dir/Vhd44780: ${VERILOG_SRCS} sim_main.cpp ${MEMS_LOC}
	verilator -Wall --cc ${VERILOG_SRCS} --exe sim_main.cpp ${VERILATOR_EXTRA_OPTS}
	make -C obj_dir -j -f Vhd44780.mk Vhd44780

.PHONY: clean clean_cont

clean:
	rm -rf obj_dir *.mem

clean_cont:
	rm .Dockerfile
