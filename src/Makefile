.PHONY: run

all: obj_dir/Vhd44780

run: obj_dir/Vhd44780
	obj_dir/Vhd44780

test: run

obj_dir/Vhd44780: hd44780.v sim_main.cpp
	/bin/bash compile_inst.sh

.PHONY: clean clean_cont

clean:
	rm -rf obj_dir

clean_cont:
	rm .Dockerfile
