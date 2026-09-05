################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include X-CUBE-AI/App/subdir.mk
-include Sentinel/profiler/subdir.mk
-include Sentinel/inference/subdir.mk
-include Sentinel/decision/subdir.mk
-include Sentinel/subdir.mk
-include Middlewares/uTKernel/sysdepend/stm32_cube/lib/libtk/cpu/stm32n6/subdir.mk
-include Middlewares/uTKernel/sysdepend/stm32_cube/lib/libtk/cpu/core/armv8m/subdir.mk
-include Middlewares/uTKernel/sysdepend/stm32_cube/lib/libtk/subdir.mk
-include Middlewares/uTKernel/sysdepend/stm32_cube/device/hal_i2c/subdir.mk
-include Middlewares/uTKernel/sysdepend/stm32_cube/device/hal_adc/sysdepend/subdir.mk
-include Middlewares/uTKernel/sysdepend/stm32_cube/device/hal_adc/subdir.mk
-include Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv8m/subdir.mk
-include Middlewares/uTKernel/sysdepend/stm32_cube/subdir.mk
-include Middlewares/uTKernel/mtkernel/lib/libtm/sysdepend/no_device/subdir.mk
-include Middlewares/uTKernel/mtkernel/lib/libtm/subdir.mk
-include Middlewares/uTKernel/mtkernel/lib/libtk/subdir.mk
-include Middlewares/uTKernel/mtkernel/kernel/usermain/subdir.mk
-include Middlewares/uTKernel/mtkernel/kernel/tstdlib/subdir.mk
-include Middlewares/uTKernel/mtkernel/kernel/tkernel/subdir.mk
-include Middlewares/uTKernel/mtkernel/kernel/sysinit/subdir.mk
-include Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/rxv2/subdir.mk
-include Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7m/subdir.mk
-include Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/subdir.mk
-include Middlewares/uTKernel/mtkernel/kernel/inittask/subdir.mk
-include Middlewares/uTKernel/mtkernel/device/ser/sysdepend/tx03_m367/subdir.mk
-include Middlewares/uTKernel/mtkernel/device/ser/sysdepend/rza2m/subdir.mk
-include Middlewares/uTKernel/mtkernel/device/ser/sysdepend/rx65n/subdir.mk
-include Middlewares/uTKernel/mtkernel/device/ser/sysdepend/rx231/subdir.mk
-include Middlewares/uTKernel/mtkernel/device/ser/subdir.mk
-include Middlewares/uTKernel/mtkernel/device/i2c/sysdepend/tx03_m367/subdir.mk
-include Middlewares/uTKernel/mtkernel/device/i2c/sysdepend/rza2m/subdir.mk
-include Middlewares/uTKernel/mtkernel/device/i2c/sysdepend/rx65n/subdir.mk
-include Middlewares/uTKernel/mtkernel/device/i2c/sysdepend/rx231/subdir.mk
-include Middlewares/uTKernel/mtkernel/device/i2c/subdir.mk
-include Middlewares/uTKernel/mtkernel/device/common/drvif/subdir.mk
-include Middlewares/uTKernel/mtkernel/device/adc/sysdepend/tx03_m367/subdir.mk
-include Middlewares/uTKernel/mtkernel/device/adc/sysdepend/rza2m/subdir.mk
-include Middlewares/uTKernel/mtkernel/device/adc/sysdepend/rx65n/subdir.mk
-include Middlewares/uTKernel/mtkernel/device/adc/sysdepend/rx231/subdir.mk
-include Middlewares/uTKernel/mtkernel/device/adc/subdir.mk
-include Middlewares/ST/AI/Npu/ll_aton/subdir.mk
-include Middlewares/ST/AI/Npu/Devices/STM32N6XX/subdir.mk
-include Drivers/STM32N6xx_HAL_Driver/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := STM_MX_till_now_AppliSecure
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
STM_MX_till_now_AppliSecure.elf \

MAP_FILES += \
STM_MX_till_now_AppliSecure.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
STM_MX_till_now_AppliSecure.list \

OBJCOPY_BIN += \
STM_MX_till_now_AppliSecure.bin \


# All Target
all: main-build

# Main-build Target
main-build: STM_MX_till_now_AppliSecure.elf secondary-outputs

# Tool invocations
STM_MX_till_now_AppliSecure.elf STM_MX_till_now_AppliSecure.map: $(OBJS) $(USER_OBJS) C:\Users\user\Documents\Sentinel-RT\STM_MX_till_now\AppliSecure\STM32N657X0HXQ_LRUN_s.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "STM_MX_till_now_AppliSecure.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m55 -T"C:\Users\user\Documents\Sentinel-RT\STM_MX_till_now\AppliSecure\STM32N657X0HXQ_LRUN_s.ld" --specs=nosys.specs -Wl,-Map="STM_MX_till_now_AppliSecure.map" -Wl,--gc-sections -static -Wl,--cmse-implib -Wl,--out-implib=./secure_nsclib.o -L../../Middlewares/ST/AI/Lib --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

STM_MX_till_now_AppliSecure.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "STM_MX_till_now_AppliSecure.list"
	@echo 'Finished building: $@'
	@echo ' '

STM_MX_till_now_AppliSecure.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "STM_MX_till_now_AppliSecure.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) STM_MX_till_now_AppliSecure.bin STM_MX_till_now_AppliSecure.elf STM_MX_till_now_AppliSecure.list STM_MX_till_now_AppliSecure.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
