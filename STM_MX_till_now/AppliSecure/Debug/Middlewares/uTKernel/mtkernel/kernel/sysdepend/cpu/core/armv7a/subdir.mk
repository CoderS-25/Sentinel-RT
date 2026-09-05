################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/cpu_cntl.c \
../Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/exc_hdl.c \
../Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/interrupt.c \
../Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/reset_main.c 

S_UPPER_SRCS += \
../Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/dispatch.S \
../Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/exc_entry.S \
../Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/int_asm.S \
../Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/reset_hdl.S \
../Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/vector_tbl.S 

OBJS += \
./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/cpu_cntl.o \
./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/dispatch.o \
./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/exc_entry.o \
./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/exc_hdl.o \
./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/int_asm.o \
./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/interrupt.o \
./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/reset_hdl.o \
./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/reset_main.o \
./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/vector_tbl.o 

S_UPPER_DEPS += \
./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/dispatch.d \
./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/exc_entry.d \
./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/int_asm.d \
./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/reset_hdl.d \
./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/vector_tbl.d 

C_DEPS += \
./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/cpu_cntl.d \
./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/exc_hdl.d \
./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/interrupt.d \
./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/reset_main.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/%.o Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/%.su Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/%.cyclo: ../Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/%.c Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m55 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32N657xx -D_STM32CUBE_DISCOVERY_N657_ -DCNF_SYSTEMAREA_END=0x34100000 -DLL_ATON_DUMP_DEBUG_API -DLL_ATON_PLATFORM=LL_ATON_PLAT_STM32N6 -DLL_ATON_OSAL=LL_ATON_OSAL_BARE_METAL -DLL_ATON_RT_MODE=LL_ATON_RT_ASYNC -DLL_ATON_SW_FALLBACK -DLL_ATON_EB_DBG_INFO -DLL_ATON_DBG_BUFFER_INFO_EXCLUDED=1 -c -I../Core/Inc -I"C:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel/mtkernel/kernel/knlinc" -I../../Secure_nsclib -I../../Middlewares/ST/AI/Npu/Devices/STM32N6XX -I../../Middlewares/ST/AI/Inc -I../../Middlewares/ST/AI/Npu/ll_aton -I../X-CUBE-AI/App -I../../Drivers/STM32N6xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32N6xx/Include -I../../Drivers/STM32N6xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Include -I../../AppliSecure/X-CUBE-AI/App -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel/include -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel/config -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Core/Inc -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Core/Inc -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel/include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -mcmse -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/%.o: ../Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/%.S Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m55 -g3 -DDEBUG -c -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel/include -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel/mtkernel/include -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel/config -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Core/Inc -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Core/Inc -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Middlewares-2f-uTKernel-2f-mtkernel-2f-kernel-2f-sysdepend-2f-cpu-2f-core-2f-armv7a

clean-Middlewares-2f-uTKernel-2f-mtkernel-2f-kernel-2f-sysdepend-2f-cpu-2f-core-2f-armv7a:
	-$(RM) ./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/cpu_cntl.cyclo ./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/cpu_cntl.d ./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/cpu_cntl.o ./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/cpu_cntl.su ./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/dispatch.d ./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/dispatch.o ./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/exc_entry.d ./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/exc_entry.o ./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/exc_hdl.cyclo ./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/exc_hdl.d ./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/exc_hdl.o ./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/exc_hdl.su ./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/int_asm.d ./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/int_asm.o ./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/interrupt.cyclo ./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/interrupt.d ./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/interrupt.o ./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/interrupt.su ./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/reset_hdl.d ./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/reset_hdl.o ./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/reset_main.cyclo ./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/reset_main.d ./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/reset_main.o ./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/reset_main.su ./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/vector_tbl.d ./Middlewares/uTKernel/mtkernel/kernel/sysdepend/cpu/core/armv7a/vector_tbl.o

.PHONY: clean-Middlewares-2f-uTKernel-2f-mtkernel-2f-kernel-2f-sysdepend-2f-cpu-2f-core-2f-armv7a

