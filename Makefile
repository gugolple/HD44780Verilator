export VER_POD_IMAGE = vertest
export PODMAN_ARGS = --rm -v $(shell pwd):/mnt
.PHONY: all run test container

SRC_DIR=src
CONTAINER_DIR=container

container:
	cd ${CONTAINER_DIR} ; make all

all:
	cd ${SRC_DIR} ; make all

run: 
	cd ${SRC_DIR} ; make run

test: 
	cd ${SRC_DIR} ; make test

exec-%:
	$(info "$(@:exec-%=%)")
	cd src ; make "$(@:exec-%=%)"

container-exec-%: container
	$(info "$(@:container-exec-%=%)")
	podman run ${PODMAN_ARGS} -t ${VER_POD_IMAGE} make "$(@:container-%=%)" 

container-run-it: container
	podman run ${PODMAN_ARGS} -it ${VER_POD_IMAGE}

container-clean:
	cd ${SRC_DIR} ; make clean

container-clean-force:
	cd ${SRC_DIR} ; make clean-force

clean-container:
	cd ${CONTAINER_DIR} ; make clean

clean-all: clean-container clean-build
