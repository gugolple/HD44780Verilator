.PHONY: run

TGT = hd447804b
export TGT
TTGT = T${TGT}

COMPILATION_PATH = obj_dir
PVTGTLib = ${COMPILATION_PATH}/${VTGTLib}

VERILATOR_EXTRA_OPTS1 = --top-module ${TGT} -Wno-INITIALDLY -Wno-UNUSED
VERILATOR_EXTRA_OPTS2 = ${VERILATOR_EXTRA_OPTS1} -Wno-UNDRIVEN -Wno-REDEFMACRO
VERILATOR_EXTRA_OPTS = ${VERILATOR_EXTRA_OPTS2} -Wno-PINMISSING

VERILATOR_CUSTOM_OVERRIDES = -CFLAGS '-I../test_srcs/verilator_overrides -DVL_VERILATED_INCLUDE="<custom_overrides.h>" -DVL_PRINTF=my_printf'
PATH_SRC = fpga_gowin_hd44780/src
VERILOG_SRCS = $(wildcard ${PATH_SRC}/*.v)

TSRC = test_srcs
TSRC_BIN = ${TSRC}/bin
TSRC_DBIN = ${TSRC}/dbin
TSRC_MAKE = ${TSRC_BIN}/Makefile
TSRC_DMAKE = ${TSRC_DBIN}/Makefile
RUN_TGT = vtest${TGT}
TRUN_TGT = ${TSRC_BIN}/${RUN_TGT}
TRUN_DTGT = ${TSRC_DBIN}/${RUN_TGT}
TEST_TGT = testmain
TSRC_TGT = ${TSRC_BIN}/${TEST_TGT}
TSRC_DTGT = ${TSRC_DBIN}/${TEST_TGT}

all: ${PVTGTLib} ${RUN_TGT}

# Non debug binaries
run: ${TRUN_TGT}
	cd ${TSRC_BIN} ; ./${RUN_TGT}

test: ${TSRC_TGT}
	cd ${TSRC_BIN} ; ./${TEST_TGT}

test-v: ${TSRC_TGT}
	cd ${TSRC_BIN} ; ./${TEST_TGT} -s

# Debug binaries
run-g: ${TSRC_DTGT}
test-g: ${TSRC_DTGT}

run-gr: ${TSRC_DTGT}
	cd ${TSRC_DBIN} ; ./${RUN_TGT}

test-gr: ${TSRC_DTGT}
	cd ${TSRC_DBIN} ; ./${TEST_TGT}

run-dbg: run-g 
	cd ${TSRC_DBIN} ; gdb ${RUN_TGT}

test-gdb: test-g
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

${PVTGTLib}: ${VERILOG_SRCS}
	verilator -Wall --cc ${VERILOG_SRCS} ${VERILATOR_EXTRA_OPTS} -lib-create ${TTGT} ${VERILATOR_CUSTOM_OVERRIDES} 
	make -C ${COMPILATION_PATH} -j -f V${TGT}.mk

.PHONY: ${TSRC_MAKE}
${TSRC_MAKE}:
	mkdir -p ${TSRC_BIN}
	cd ${TSRC} && autoreconf -f -i && cd ../${TSRC_BIN} && ../configure

${TSRC_TGT}: ${TSRC_MAKE} ${PVTGTLib}
	cd ${TSRC_BIN} && make -j `nproc`

.PHONY: ${TSRC_DMAKE}
${TSRC_DMAKE}:
	mkdir -p ${TSRC_DBIN}
	cd ${TSRC} && autoreconf -f -i && cd ../${TSRC_DBIN} && ../configure --enable-debug

${TSRC_DTGT}: ${TSRC_DMAKE} ${PVTGTLib}
	cd ${TSRC_DBIN} && make -j `nproc`

${TRUN_TGT}: ${TSRC_TGT}

${TRUN_DTGT}: ${TSRC_DTGT}
