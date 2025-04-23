.PHONY: all

all: .Dockerfile

.Dockerfile: Dockerfile ../Makefile
	podman build -t ${VER_POD_IMAGE} -f Dockerfile ${PODMAN_ARGS}
	touch -m .Dockerfile

clean:
	rm -f .Dockerfile
