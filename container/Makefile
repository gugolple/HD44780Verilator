.PHONY: all

all: .Dockerfile

.Dockerfile: Dockerfile ../Makefile
	podman build -t ${VER_POD_IMAGE} -f Dockerfile ${PODMAN_ARGS}
	touch -m .Dockerfile

clean:
	rm -f .Dockerfile

/usr/local/share/verilator/include/verilated.h:
	unset VERILATOR_ROOT && cd verilator && autoconf && ./configure && make -j `nproc` && make install 

verilatortgt: /usr/local/share/verilator/include/verilated.h

/usr/local/lib/libCatch2.a:
	cd Catch2 && cmake -S . -B build && cd build && make -j`nproc` && make install

catch2tgt: /usr/local/lib/libCatch2.a

deps: verilatortgt catch2tgt
