FROM catthehacker/ubuntu:act-latest

# Standar requirements for overall
RUN apt update && apt install -y build-essential perl-doc autotools-dev gdb cgdb catch2 git make autoconf g++ cmake
# Requirements for compiling verilator
RUN apt-get install -y git help2man perl python3 make autoconf g++ flex bison ccache libgoogle-perftools-dev numactl perl-doc libfl2 libfl-dev zlib1g zlib1g-dev

WORKDIR /mnt

RUN make deps
