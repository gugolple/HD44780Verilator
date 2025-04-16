FROM catthehacker/ubuntu:act-latest

RUN apt update && apt install -y build-essential perl-doc autotools-dev
RUN apt-get install -y git help2man perl python3 make autoconf g++ flex bison ccache && \
apt-get install -y libgoogle-perftools-dev numactl perl-doc && \
apt-get install -y libfl2 && \
apt-get install -y libfl-dev && \
apt-get install -y zlib1g zlib1g-dev

RUN git clone https://github.com/verilator/verilator && \
unset VERILATOR_ROOT && \
cd verilator && \
autoconf && \
./configure && \
make -j `nproc` && \
make install && \
cd .. && \
rm -rvf verilator

WORKDIR /mnt
