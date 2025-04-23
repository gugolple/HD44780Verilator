if DEBUG
AM_CFLAGS = -Og -Wall -Wextra -g3 -ggdb
AM_CXXFLAGS = -Og -Wall -Wextra -g3 -ggdb
else
AM_CFLAGS =  -Os -Wall -Wextra
AM_CXXFLAGS = -Os -Wall -Wextra
endif

INCLUDE_DIRS = \
	-I$(srcdir)/../obj_dir/ \
	-I/usr/share/verilator/include/ \
	-I/usr/local/share/verilator/include/

# General block of code to be shared for all targets
noinst_LIBRARIES = libgen.a

libgen_a_SOURCES = \
	$(srcdir)/src/HD44780.cpp \
	$(srcdir)/src/HD44780General.cpp \
	$(srcdir)/src/TestFrameWork.cpp \
	$(srcdir)/src/TestFrameWorkHD44780.cpp

libgen_a_CPPFLAGS = \
	-Wno-sign-compare \
	-Wno-unused-parameter \
	$(INCLUDE_DIRS)

# Targets to be built
bin_PROGRAMS = vtesthd44780 testmain

vtesthd44780_CPPFLAGS = \
	-Wno-sign-compare \
	-Wno-unused-parameter \
	$(INCLUDE_DIRS)

vtesthd44780_LDADD = \
	$(srcdir)/../obj_dir/libThd44780.a \
	libgen.a

vtesthd44780_SOURCES = \
	$(srcdir)/src/sim_main.cpp

catch2LDADD = \
	-lCatch2Main \
	-lCatch2

testmain_LDADD = \
	$(catch2LDADD)

testmain_SOURCES = \
	$(srcdir)/src/testmain.cpp

dist_doc_DATA = README
