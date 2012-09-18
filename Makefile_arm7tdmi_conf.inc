BOARD          = arm7tdmi_generic
MCU_TARGET     = arm7tdmi 
MCU_OPTS       = -marm -mthumb-interwork
OPTIMIZE       = -Os
FLASHCMD       = $(OPENOCD) -f openocd.cfg \
	             -c "init" \
	             -c "halt" \
	             -c "flash write_image erase $(1) 0 bin" \
	             -c "reset run" \
	             -c "shutdown"

override CFLAGS_A	= $(COMMON_FLAGS) \
                      -MF$(DEP_DIR)$(patsubst %.o,%.d,$(notdir $(1))) 
override CFLAGS 	= $(COMMON_FLAGS) \
                      -MF$(DEP_DIR)$(patsubst %.o,%.d,$(notdir $@))
                      
override LDFLAGS       =  -g -Wl,--gc-sections \
                         -Wl,--entry=reset_isr  \
                         -Wl,-Map,# no space at the end
                          
override ASFLAGS	   =  -mcpu=$(MCU_TARGET) 

