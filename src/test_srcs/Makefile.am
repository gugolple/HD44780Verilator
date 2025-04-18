bin_PROGRAMS = vtesthd44780
INCLUDE_DIRS = -I$(srcdir)/../obj_dir/ -I/usr/share/verilator/include/ -I/usr/local/share/verilator/include/
vtesthd44780_CPPFLAGS = -Os -std=c++20 -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter $(INCLUDE_DIRS)
vtesthd44780_LDADD = $(srcdir)/../obj_dir/libThd44780.a
vtesthd44780_SOURCES = src/sim_main.cpp
dist_doc_DATA = README
