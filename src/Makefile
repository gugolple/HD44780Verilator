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
TSRC_DBIN = ${TSRC}/dbin
TSRC_MAKE = ${TSRC_BIN}/Makefile
TSRC_DMAKE = ${TSRC_DBIN}/Makefile
RUN_TGT = vtesthd44780
TRUN_TGT = ${TSRC_BIN}/${RUN_TGT}
TRUN_DTGT = ${TSRC_DBIN}/${RUN_TGT}
TEST_TGT = testmain
TSRC_TGT = ${TSRC_BIN}/${TEST_TGT}
TSRC_DTGT = ${TSRC_DBIN}/${TEST_TGT}

MEMS_LOC = $(patsubst %, ${TSRC_BIN}/%, $(notdir ${MEMS}))
MEMS_DLOC = $(patsubst %, ${TSRC_DBIN}/%, $(notdir ${MEMS}))

all: ${PVTGTLib} ${RUN_TGT}

run: ${TRUN_TGT}
	cd ${TSRC_BIN} ; ./${RUN_TGT}

run-dbg: ${TSRC_DTGT}
	cd ${TSRC_DBIN} ; gdb ${RUN_TGT}

test: ${TSRC_TGT}
	cd ${TSRC_BIN} ; ./${TEST_TGT}

test-dbg: ${TSRC_DTGT}
	cd ${TSRC_DBIN} ; gdb ${TEST_TGT}

.PHONY: clean
clean-verilator:
	rm -rf obj_dir

clean-srcs:
	cd ${TSRC_BIN} && make clean

clean-srcs-force:
	rm -rf ${TSRC_BIN}

clean: clean-verilator clean-srcs

clean-force: clean-verilator clean-srcs-force

${TSRC_BIN}/%.mem: ${PATH_SRC}/%.mem 
	cp $< $@

${TSRC_DBIN}/%.mem: ${PATH_SRC}/%.mem 
	cp $< $@

${PVTGTLib}: ${VERILOG_SRCS}
	verilator -Wall --cc ${VERILOG_SRCS} ${VERILATOR_EXTRA_OPTS} -lib-create ${TTGT} 
	make -C ${COMPILATION_PATH} -j -f V${TGT}.mk

.PHONY: ${TSRC_MAKE}
${TSRC_MAKE}:
	mkdir -p ${TSRC_BIN}
	cd ${TSRC} && autoreconf -f -i && cd ../${TSRC_BIN} && ../configure

.PHONY: ${TSRC_TGT}
${TSRC_TGT}: ${TSRC_MAKE} ${MEMS_LOC} ${PVTGTLib}
	cd ${TSRC_BIN} && make -j `nproc`

.PHONY: ${TSRC_DMAKE}
${TSRC_DMAKE}:
	mkdir -p ${TSRC_DBIN}
	cd ${TSRC} && autoreconf -f -i && cd ../${TSRC_DBIN} && ../configure --enable-debug

.PHONY: ${TSRC_DTGT}
${TSRC_DTGT}: ${TSRC_DMAKE} ${MEMS_DLOC} ${PVTGTLib}
	cd ${TSRC_DBIN} && make -j `nproc`

.PHONY: ${TRUN_TGT} ${TRUN_DTGT}
TRUN_TGT: ${TSRC_TGT}
TRUN_DTGT: ${TSRC_DTGT}
