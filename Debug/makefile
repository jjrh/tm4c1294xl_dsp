################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CG_TOOL_ROOT := /home/keith/ti/ccsv6/tools/compiler/arm_5.1.10

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./arm_fft_bin_data.obj" \
"./arm_fft_bin_example_f32.obj" \
"./tm4c1294ncpdt_startup_ccs.obj" \
"../tm4c1294ncpdt.cmd" \
-l"libc.a" \
-l/home/keith/arm/CMSIS/Lib/CCS/M4/dsplib-cm4f.lib \

-include ../makefile.init

RM := rm -rf
RMDIR := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
ti_cortex_m4_arm_fft_bin_example.out \

EXE_OUTPUTS__QUOTED += \
"ti_cortex_m4_arm_fft_bin_example.out" \

BIN_OUTPUTS += \
ti_cortex_m4_arm_fft_bin_example.hex \

BIN_OUTPUTS__QUOTED += \
"ti_cortex_m4_arm_fft_bin_example.hex" \


# All Target
all: ti_cortex_m4_arm_fft_bin_example.out

# Tool invocations
ti_cortex_m4_arm_fft_bin_example.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"/home/keith/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -Ooff -g --gcc --define=ccs="ccs" --define=TARGET_IS_TM4C129_RA1 --define=ARM_MATH_CM4 --define=__FPU_PRESENT=1 --define=_LINKAGE --define=_CODE_ACCESS="" --define=_DATA_ACCESS="" --define=PART_TM4C1294NCPDT --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --ual -z -m"ti_cortex_m4_arm_fft_bin_example.map" --heap_size=0 --stack_size=1024 -i"/home/keith/ti/ccsv6/tools/compiler/arm_5.1.10/lib" -i"/home/keith/ti/ccsv6/tools/compiler/arm_5.1.10/include" --reread_libs --warn_sections --display_error_number --diag_wrap=off --xml_link_info="ti_cortex_m4_arm_fft_bin_example_linkInfo.xml" --rom_model -o "ti_cortex_m4_arm_fft_bin_example.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

ti_cortex_m4_arm_fft_bin_example.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: ARM Hex Utility'
	"/home/keith/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armhex"  -o "ti_cortex_m4_arm_fft_bin_example.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "arm_fft_bin_data.pp" "arm_fft_bin_example_f32.pp" "tm4c1294ncpdt_startup_ccs.pp" 
	-$(RM) "arm_fft_bin_data.obj" "arm_fft_bin_example_f32.obj" "tm4c1294ncpdt_startup_ccs.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

