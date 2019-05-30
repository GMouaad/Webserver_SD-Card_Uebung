################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
drivers/%.obj: ../drivers/%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/Tools/TexasInstruments/ccs820/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --include_path="C:/Tools/TexasInstruments/TivaWare_C_Series-2.1.4.178" --include_path="C:/Tools/TexasInstruments/ccs820/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --include_path="C:/HTWG_Programme/Projekt_VS_EIB_6/WEBSERVER_SDCARD_Uebung" --include_path="C:/Tools/TexasInstruments/TivaWare_C_Series-2.1.4.178/examples/boards/ek-tm4c129exl" --include_path="C:/Tools/TexasInstruments/TivaWare_C_Series-2.1.4.178/third_party/lwip-1.4.1/src/include" --include_path="C:/Tools/TexasInstruments/TivaWare_C_Series-2.1.4.178/third_party/lwip-1.4.1/src/include/ipv4" --include_path="C:/Tools/TexasInstruments/TivaWare_C_Series-2.1.4.178/third_party/lwip-1.4.1/ports/tiva-tm4c129/include" --include_path="C:/Tools/TexasInstruments/TivaWare_C_Series-2.1.4.178/third_party/lwip-1.4.1/apps" --include_path="C:/Tools/TexasInstruments/TivaWare_C_Series-2.1.4.178/third_party" --advice:power="all" --define=ccs="ccs" --define=PART_TM4C129ENCPDT --define=TARGET_IS_TM4C129_RA0 -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --ual --preproc_with_compile --preproc_dependency="drivers/$(basename $(<F)).d_raw" --obj_directory="drivers" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

drivers/pinout.obj: C:/Tools/TexasInstruments/TivaWare_C_Series-2.1.4.178/examples/boards/ek-tm4c129exl/drivers/pinout.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/Tools/TexasInstruments/ccs820/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --include_path="C:/Tools/TexasInstruments/TivaWare_C_Series-2.1.4.178" --include_path="C:/Tools/TexasInstruments/ccs820/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --include_path="C:/HTWG_Programme/Projekt_VS_EIB_6/WEBSERVER_SDCARD_Uebung" --include_path="C:/Tools/TexasInstruments/TivaWare_C_Series-2.1.4.178/examples/boards/ek-tm4c129exl" --include_path="C:/Tools/TexasInstruments/TivaWare_C_Series-2.1.4.178/third_party/lwip-1.4.1/src/include" --include_path="C:/Tools/TexasInstruments/TivaWare_C_Series-2.1.4.178/third_party/lwip-1.4.1/src/include/ipv4" --include_path="C:/Tools/TexasInstruments/TivaWare_C_Series-2.1.4.178/third_party/lwip-1.4.1/ports/tiva-tm4c129/include" --include_path="C:/Tools/TexasInstruments/TivaWare_C_Series-2.1.4.178/third_party/lwip-1.4.1/apps" --include_path="C:/Tools/TexasInstruments/TivaWare_C_Series-2.1.4.178/third_party" --advice:power="all" --define=ccs="ccs" --define=PART_TM4C129ENCPDT --define=TARGET_IS_TM4C129_RA0 -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --ual --preproc_with_compile --preproc_dependency="drivers/$(basename $(<F)).d_raw" --obj_directory="drivers" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


