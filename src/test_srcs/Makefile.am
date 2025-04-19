if DEBUG
AM_CFLAGS = -Og -Wall -Wextra -g3 -ggdb
AM_CXXFLAGS = -Og -Wall -Wextra -g3 -ggdb
else
AM_CFLAGS =  -Os -Wall -Wextra
AM_CXXFLAGS = -Os -Wall -Wextra
endif

bin_PROGRAMS = vtesthd44780
INCLUDE_DIRS = -I$(srcdir)/../obj_dir/ -I/usr/share/verilator/include/ -I/usr/local/share/verilator/include/
vtesthd44780_CPPFLAGS = -Wno-sign-compare -Wno-unused-parameter $(INCLUDE_DIRS)
vtesthd44780_LDADD = $(srcdir)/../obj_dir/libThd44780.a
vtesthd44780_SOURCES = src/sim_main.cpp
dist_doc_DATA = README
