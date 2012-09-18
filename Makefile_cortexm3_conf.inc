BOARD          = lm3s9b90_cm3
MCU_TARGET     = cortex-m3  
MCU_OPTS       = -mthumb 
OPTIMIZE       = -Os -fomit-frame-pointer  
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
                      
override LDFLAGS       = -g -T lm3s9b90.ld -Wl,--gc-sections    \
                         -Wl,--entry=reset_isr  \
                         -Wl,-Map,# no space at the end
                          
override ASFLAGS	   =  -mcpu=$(MCU_TARGET) 

