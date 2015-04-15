################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
arm_fft_bin_data.obj: ../arm_fft_bin_data.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/keith/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -Ooff --include_path="/home/keith/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="/home/keith/Documents/School/TCES 430/TivaWare" --include_path="/home/keith/arm/CMSIS/Include" -g --gcc --define=ccs="ccs" --define=TARGET_IS_TM4C129_RA1 --define=ARM_MATH_CM4 --define=__FPU_PRESENT=1 --define=_LINKAGE --define=_CODE_ACCESS="" --define=_DATA_ACCESS="" --define=PART_TM4C1294NCPDT --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="arm_fft_bin_data.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

arm_fft_bin_example_f32.obj: ../arm_fft_bin_example_f32.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/keith/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -Ooff --include_path="/home/keith/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="/home/keith/Documents/School/TCES 430/TivaWare" --include_path="/home/keith/arm/CMSIS/Include" -g --gcc --define=ccs="ccs" --define=TARGET_IS_TM4C129_RA1 --define=ARM_MATH_CM4 --define=__FPU_PRESENT=1 --define=_LINKAGE --define=_CODE_ACCESS="" --define=_DATA_ACCESS="" --define=PART_TM4C1294NCPDT --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="arm_fft_bin_example_f32.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

tm4c1294ncpdt_startup_ccs.obj: ../tm4c1294ncpdt_startup_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/keith/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -Ooff --include_path="/home/keith/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="/home/keith/Documents/School/TCES 430/TivaWare" --include_path="/home/keith/arm/CMSIS/Include" -g --gcc --define=ccs="ccs" --define=TARGET_IS_TM4C129_RA1 --define=ARM_MATH_CM4 --define=__FPU_PRESENT=1 --define=_LINKAGE --define=_CODE_ACCESS="" --define=_DATA_ACCESS="" --define=PART_TM4C1294NCPDT --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="tm4c1294ncpdt_startup_ccs.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


