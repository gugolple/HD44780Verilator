GLOBAL_CFLAGS = \
    -Wall \
    -Wextra \
    -std=c++20

if DEBUG
AM_CFLAGS = -Og  -g3 -ggdb $(GLOBAL_CFLAGS)
AM_CXXFLAGS = -Og -g3 -ggdb $(GLOBAL_CFLAGS)
else
AM_CFLAGS =  -Os $(GLOBAL_CFLAGS)
AM_CXXFLAGS = -Os $(GLOBAL_CFLAGS)
endif

INCLUDE_DIRS = \
        -I$(srcdir)/verilator_overrides/ \
	-I$(srcdir)/../obj_dir/ \
	-I/usr/share/verilator/include/ \
	-I/usr/local/share/verilator/include/

################################################################################
# Lib target 
# Made so we dont need to recompile the shared files
################################################################################
# General block of code to be shared for all targets
noinst_LIBRARIES = libgen.a

libgen_a_SOURCES = \
	$(srcdir)/src/HD44780.cpp \
	$(srcdir)/src/HD44780General.cpp \
	$(srcdir)/src/TestFrameWork.cpp \
	$(srcdir)/src/TestFrameWorkHD44780.cpp

LIBGENFLAGS = \
	-Wno-sign-compare \
	-Wno-unused-parameter \
	$(INCLUDE_DIRS)

catch2LDADD = \
	-lCatch2Main \
	-lCatch2

LIBGENLDADD = \
	$(srcdir)/../obj_dir/libT$(TGT).a \
	$(catch2LDADD) \
	libgen.a

libgen_a_CPPFLAGS = \
	$(LIBGENFLAGS)

# Targets to be built
bin_PROGRAMS = vtesthd44780 testmain

################################################################################
# Default run target
# Expected for more interactive operations
################################################################################
vtesthd44780_CPPFLAGS = \
	$(LIBGENFLAGS)

vtesthd44780_LDADD = \
	$(LIBGENLDADD)

vtesthd44780_SOURCES = \
        $(srcdir)/verilator_overrides/custom_overrides.cpp \
	$(srcdir)/src/sim_main.cpp

################################################################################
# Unit test target 
################################################################################
testmain_CPPFLAGS = \
	$(LIBGENFLAGS)

testmain_LDADD = \
	$(LIBGENLDADD)

testmain_SOURCES = \
        $(srcdir)/verilator_overrides/custom_overrides.cpp \
	$(srcdir)/src/testmain.cpp

dist_doc_DATA = README
