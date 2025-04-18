.PHONY: run

TGT = hd44780
TTGT = T${TGT}

COMPILATION_PATH = obj_dir
PVTGTLib = ${COMPILATION_PATH}/${VTGTLib}

VERILATOR_EXTRA_OPTS = --top-module ${TGT} -Wno-INITIALDLY -Wno-UNUSED -Wno-UNDRIVEN 
PATH_SRC = fpga_gowin_hd44780/src
VERILOG_SRCS = $(wildcard ${PATH_SRC}/*.v)
MEMS = $(wildcard ${PATH_SRC}/*.mem)

TSRC = test_srcs
TSRC_BIN = ${TSRC}/bin
TSRC_MAKE = ${TSRC_BIN}/Makefile
TEST_TGT = vtesthd44780 
TSRC_TGT = ${TSRC_BIN}/${TEST_TGT}
MEMS_LOC = $(patsubst %, ${TSRC_BIN}/%, $(notdir ${MEMS}))

$(info $(TSRC_MAKE))

${TSRC_BIN}/%.mem: ${PATH_SRC}/%.mem 
	cp $< $@

all: ${PVTGTLib}

run: ${TSRC_TGT}
	cd ${TSRC_BIN} ; ./${TEST_TGT}

test: run

${PVTGTLib}: ${VERILOG_SRCS}
	verilator -Wall --cc ${VERILOG_SRCS} ${VERILATOR_EXTRA_OPTS} -lib-create ${TTGT} 
	make -C ${COMPILATION_PATH} -j -f Vhd44780.mk

${TSRC_MAKE}:
	mkdir -p ${TSRC_BIN}
	cd ${TSRC} && autoreconf -f -i && cd ../${TSRC_BIN} && ../configure

${TSRC_TGT}: ${TSRC_MAKE} ${MEMS_LOC} ${PVTGTLib}
	cd ${TSRC_BIN} && make -j `nproc`

.PHONY: clean clean_cont

clean:
	rm -rf obj_dir *.mem
	cd ${TSRC_BIN} ; make clean

clean_cont:
	rm .Dockerfile
