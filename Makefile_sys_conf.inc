TOOLCHAIN      = arm-elf-#
DEBUG          = -gdwarf-2
WARNING        = -pedantic -Wall -Wstrict-prototypes -Werror
DEFS           = -D$(call uc, $(subst -,_,$(MCU_TARGET)))
DEP_DIR        = deps/$(BOARD)/#
TEST_DIR       = test/#
BIN_DIR        = bin/$(BOARD)/#
TESTSRC_DIR    = test_src/#
ERASECMD       =
TESTPORT       = /dev/ttyUSB2
TESTPORTBAUDR  = 115200
TESTLOG_DIR    = testlog/#
TESTPREFIX     = nessie-#
SPEEDTOOL      = host/get_performance.rb
SPEEDLOG_DIR   = speed_log/$(BOARD)/#
SPEEDPREFIX    =
SPEEDCMD       = performance
SIZE_DIR       = size_log/#
LIST_DIR       = listings/$(BOARD)/#
STAT_DIR       = stats/#
AUTOASM_DIR    = autoasm/$(BOARD)/#
LIB_DIR        = lib/#
LIB_NAME       = libcrypto.a
AUTOASM_OPT    = -S
CSTD           = gnu99
LIBS           = c gcc

COMMON_FLAGS   =   \
                  $(DEBUG) $(WARNING) -std=$(CSTD) $(OPTIMIZE) \
                  -mcpu=$(MCU_TARGET) $(MCU_OPTS) $(DEFS) -MMD
		  
                #  -ffunction-sections -fdata-sections          \

SIZESTAT_FILE = sizestats.txt

CC	            = $(TOOLCHAIN)gcc
OBJCOPY         = $(TOOLCHAIN)objcopy
OBJDUMP         = $(TOOLCHAIN)objdump
SIZE            = $(TOOLCHAIN)size
OPENOCD         = openocd
READELF         = readelf
AR              = $(TOOLCHAIN)ar
CHMOD           = chmod
CHOWN           = chown
CHGRP           = chgrp
RM              = rm
RUBY            = ruby
GET_TEST        = host/get_test.rb
MAKE            = make
MAKE2GRAPH      = ~/bin/make2graph.rb
TWOPI           = twopi
