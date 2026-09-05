################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/cpu_cntl.c \
../Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/interrupt.c \
../Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/sys_start.c 

S_UPPER_SRCS += \
../Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/dispatch.S 

OBJS += \
./Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/cpu_cntl.o \
./Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/dispatch.o \
./Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/interrupt.o \
./Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/sys_start.o 

S_UPPER_DEPS += \
./Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/dispatch.d 

C_DEPS += \
./Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/cpu_cntl.d \
./Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/interrupt.d \
./Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/sys_start.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/%.o Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/%.su Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/%.cyclo: ../Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/%.c Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m55 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32N657xx -DLL_ATON_DUMP_DEBUG_API -DLL_ATON_PLATFORM=LL_ATON_PLAT_STM32N6 -DLL_ATON_OSAL=LL_ATON_OSAL_BARE_METAL -DLL_ATON_RT_MODE=LL_ATON_RT_ASYNC -DLL_ATON_SW_FALLBACK -DLL_ATON_EB_DBG_INFO -DLL_ATON_DBG_BUFFER_INFO_EXCLUDED=1 -c -I../Core/Inc -I../../Secure_nsclib -I../../Middlewares/ST/AI/Npu/Devices/STM32N6XX -I../../Middlewares/ST/AI/Inc -I../../Middlewares/ST/AI/Npu/ll_aton -I../X-CUBE-AI/App -I../../Drivers/STM32N6xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32N6xx/Include -I../../Drivers/STM32N6xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Include -I../../AppliSecure/X-CUBE-AI/App -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel/include -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel/config -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel/include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -mcmse -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/%.o: ../Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/%.S Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m55 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Middlewares-2f-uTKernel-2f-sysdepend-2f-stm32_cube-2f-cpu-2f-core-2f-armv7m

clean-Middlewares-2f-uTKernel-2f-sysdepend-2f-stm32_cube-2f-cpu-2f-core-2f-armv7m:
	-$(RM) ./Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/cpu_cntl.cyclo ./Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/cpu_cntl.d ./Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/cpu_cntl.o ./Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/cpu_cntl.su ./Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/dispatch.d ./Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/dispatch.o ./Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/interrupt.cyclo ./Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/interrupt.d ./Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/interrupt.o ./Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/interrupt.su ./Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/sys_start.cyclo ./Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/sys_start.d ./Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/sys_start.o ./Middlewares/uTKernel/sysdepend/stm32_cube/cpu/core/armv7m/sys_start.su

.PHONY: clean-Middlewares-2f-uTKernel-2f-sysdepend-2f-stm32_cube-2f-cpu-2f-core-2f-armv7m

