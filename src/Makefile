.PHONY: run

VERILATOR_EXTRA_OPTS = -Wno-INITIALDLY -Wno-UNUSED

all: obj_dir/Vhd44780

run: obj_dir/Vhd44780
	obj_dir/Vhd44780

test: run

obj_dir/Vhd44780: hd44780.v sim_main.cpp
	verilator -Wall --cc hd44780.v --exe sim_main.cpp ${VERILATOR_EXTRA_OPTS}
	make -C obj_dir -j -f Vhd44780.mk Vhd44780

.PHONY: clean clean_cont

clean:
	rm -rf obj_dir

clean_cont:
	rm .Dockerfile
