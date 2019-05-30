################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/Tools/TexasInstruments/ccs820/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./I2C_LightSens.obj" \
"./enet_fs.obj" \
"./enet_lwip.obj" \
"./startup_ccs.obj" \
"./webserv_appl_fcns.obj" \
"./drivers/CFAF128128B0145T.obj" \
"./drivers/pinout.obj" \
"./third_party/fatfs/port/mmc-ek-tm4c1294xl.obj" \
"./third_party/fatfs/src/ff.obj" \
"./third_party/lwip-1.4.1/apps/httpserver_raw/httpd.obj" \
"./utils/locator.obj" \
"./utils/lwiplib.obj" \
"./utils/uartstdio.obj" \
"./utils/ustdlib.obj" \
"../enet_lwip_ccs.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \
-l"C:/Tools/TexasInstruments/TivaWare_C_Series-2.1.4.178/driverlib/ccs/Debug/driverlib.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include drivers/subdir_vars.mk
-include third_party/fatfs/port/subdir_vars.mk
-include third_party/fatfs/src/subdir_vars.mk
-include third_party/lwip-1.4.1/apps/httpserver_raw/subdir_vars.mk
-include utils/subdir_vars.mk
-include subdir_rules.mk
-include drivers/subdir_rules.mk
-include third_party/fatfs/port/subdir_rules.mk
-include third_party/fatfs/src/subdir_rules.mk
-include third_party/lwip-1.4.1/apps/httpserver_raw/subdir_rules.mk
-include utils/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
WEBSERVER_SDCARD_Uebung.out \

EXE_OUTPUTS__QUOTED += \
"WEBSERVER_SDCARD_Uebung.out" \

BIN_OUTPUTS += \
WEBSERVER_SDCARD_Uebung.bin \

BIN_OUTPUTS__QUOTED += \
"WEBSERVER_SDCARD_Uebung.bin" \


# All Target
all: WEBSERVER_SDCARD_Uebung.out

# Tool invocations
WEBSERVER_SDCARD_Uebung.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: ARM Linker'
	"C:/Tools/TexasInstruments/ccs820/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --advice:power="all" --define=ccs="ccs" --define=PART_TM4C129ENCPDT --define=TARGET_IS_TM4C129_RA0 -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --ual -z -m"enet_lwip_ccs.map" --heap_size=1024 --stack_size=2048 -i"C:/Tools/TexasInstruments/ccs820/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/lib" -i"C:/Tools/TexasInstruments/ccs820/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --reread_libs --diag_wrap=off --display_error_number --warn_sections --xml_link_info="WEBSERVER_SDCARD_Uebung_linkInfo.xml" --rom_model -o "WEBSERVER_SDCARD_Uebung.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

WEBSERVER_SDCARD_Uebung.bin: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: ARM Hex Utility'
	"C:/Tools/TexasInstruments/ccs820/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armhex"  -o "WEBSERVER_SDCARD_Uebung.bin" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "I2C_LightSens.obj" "enet_fs.obj" "enet_lwip.obj" "startup_ccs.obj" "webserv_appl_fcns.obj" "drivers\CFAF128128B0145T.obj" "drivers\pinout.obj" "third_party\fatfs\port\mmc-ek-tm4c1294xl.obj" "third_party\fatfs\src\ff.obj" "third_party\lwip-1.4.1\apps\httpserver_raw\httpd.obj" "utils\locator.obj" "utils\lwiplib.obj" "utils\uartstdio.obj" "utils\ustdlib.obj" 
	-$(RM) "I2C_LightSens.d" "enet_fs.d" "enet_lwip.d" "startup_ccs.d" "webserv_appl_fcns.d" "drivers\CFAF128128B0145T.d" "drivers\pinout.d" "third_party\fatfs\port\mmc-ek-tm4c1294xl.d" "third_party\fatfs\src\ff.d" "third_party\lwip-1.4.1\apps\httpserver_raw\httpd.d" "utils\locator.d" "utils\lwiplib.d" "utils\uartstdio.d" "utils\ustdlib.d" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-"C:/Tools/TexasInstruments/ccs820/ccsv8/utils/tiobj2bin/tiobj2bin" "WEBSERVER_SDCARD_Uebung.out" "WEBSERVER_SDCARD_Uebung.bin" "C:/Tools/TexasInstruments/ccs820/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armofd" "C:/Tools/TexasInstruments/ccs820/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armhex" "C:/Tools/TexasInstruments/ccs820/ccsv8/utils/tiobj2bin/mkhex4bin"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

