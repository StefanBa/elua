##############################################################################
## FHNW			Studiengang EIT
## Projekt5		Android-basiertes Home Automation System
## Author		Stefan Baumann, stefan.baumann1@students.fhnw.ch
## Release		Datum: 20.01.2011
## Note			Rule program und was dazu gehört (OpenOCD Options) wurde vom
##				Unterricht (Modul mc2) entnommen, was wiederum ursprünglich
##				von FreeRTOS stammt.
##############################################################################

BOARD = SARHA
#BOARD = ek-lm3s9b92
#BOARD = ek-lm3s8962

#CPU = lm3s9d92
CPU = lm3s9b92
#CPU = lm3s8962

FLASHCONFIG = ek-lm3s9b92

TARGET = elua_lua_$(CPU)

OPENOCD = openocd
LM = lmflash

#OOCD_INIT  = -f interface/luminary.cfg
OOCD_INIT  += -f board/ek-lm3s8962.cfg
OOCD_INIT  += -c "jtag_khz 3000"
	#OOCD_INIT  = -f interface/olimex-arm-usb-ocd.cfg
	#OOCD_INIT  += -f target/stm32.cfg
	#OOCD_INIT  += -c "jtag_khz 1200"
OOCD_INIT  += -c init
OOCD_INIT  += -c "reset init"
	#OOCD_INIT  += -c "verify_ircapture disable"

OOCD_FLASH = -c "reset halt"
OOCD_FLASH += -c "targets"
OOCD_FLASH += -c "flash write_image erase $(TARGET).bin"
OOCD_FLASH += -c "verify_image $(TARGET).bin"
OOCD_FLASH += -c "reset run"
OOCD_FLASH += -c shutdown

SCONS_BUILD =  board=$(BOARD)
SCONS_BUILD += cpu=$(CPU)
SCONS_BUILD += toolchain=codesourcery
SCONS_BUILD += romfs=compile
SCONS_BUILD += optram=1
SCONS_BUILD += prog

LM_FLASH = --quick-set=$(FLASHCONFIG)
LM_FLASH += --erase all
LM_FLASH += --reset
LM_FLASH += $(TARGET).bin

all:

	xcopy /y $(CWD)\..\ControlUnitGit\src $(CWD)\romfs
	scons $(SCONS_BUILD)

program_ocd: $(TARGET).bin
	$(OPENOCD)  $(OOCD_INIT) $(OOCD_FLASH)

program_lm: $(TARGET).bin
	$(LM)  $(LM_FLASH)

#gdb:
#	$(OPENOCD)  $(OOCD_INIT)


	
