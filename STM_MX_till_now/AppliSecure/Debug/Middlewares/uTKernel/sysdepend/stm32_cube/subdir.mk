################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/uTKernel/sysdepend/stm32_cube/devinit.c \
../Middlewares/uTKernel/sysdepend/stm32_cube/hw_setting.c \
../Middlewares/uTKernel/sysdepend/stm32_cube/power_save.c 

OBJS += \
./Middlewares/uTKernel/sysdepend/stm32_cube/devinit.o \
./Middlewares/uTKernel/sysdepend/stm32_cube/hw_setting.o \
./Middlewares/uTKernel/sysdepend/stm32_cube/power_save.o 

C_DEPS += \
./Middlewares/uTKernel/sysdepend/stm32_cube/devinit.d \
./Middlewares/uTKernel/sysdepend/stm32_cube/hw_setting.d \
./Middlewares/uTKernel/sysdepend/stm32_cube/power_save.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/uTKernel/sysdepend/stm32_cube/%.o Middlewares/uTKernel/sysdepend/stm32_cube/%.su Middlewares/uTKernel/sysdepend/stm32_cube/%.cyclo: ../Middlewares/uTKernel/sysdepend/stm32_cube/%.c Middlewares/uTKernel/sysdepend/stm32_cube/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m55 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32N657xx -D_STM32CUBE_DISCOVERY_N657_ -DCNF_SYSTEMAREA_END=0x34100000 -DLL_ATON_DUMP_DEBUG_API -DLL_ATON_PLATFORM=LL_ATON_PLAT_STM32N6 -DLL_ATON_OSAL=LL_ATON_OSAL_BARE_METAL -DLL_ATON_RT_MODE=LL_ATON_RT_ASYNC -DLL_ATON_SW_FALLBACK -DLL_ATON_EB_DBG_INFO -DLL_ATON_DBG_BUFFER_INFO_EXCLUDED=1 -c -I../Core/Inc -I"C:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel/mtkernel/kernel/knlinc" -I../../Secure_nsclib -I../../Middlewares/ST/AI/Npu/Devices/STM32N6XX -I../../Middlewares/ST/AI/Inc -I../../Middlewares/ST/AI/Npu/ll_aton -I../X-CUBE-AI/App -I../../Drivers/STM32N6xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32N6xx/Include -I../../Drivers/STM32N6xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Include -I../../AppliSecure/X-CUBE-AI/App -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel/include -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel/config -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Core/Inc -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Core/Inc -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel/include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -mcmse -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-uTKernel-2f-sysdepend-2f-stm32_cube

clean-Middlewares-2f-uTKernel-2f-sysdepend-2f-stm32_cube:
	-$(RM) ./Middlewares/uTKernel/sysdepend/stm32_cube/devinit.cyclo ./Middlewares/uTKernel/sysdepend/stm32_cube/devinit.d ./Middlewares/uTKernel/sysdepend/stm32_cube/devinit.o ./Middlewares/uTKernel/sysdepend/stm32_cube/devinit.su ./Middlewares/uTKernel/sysdepend/stm32_cube/hw_setting.cyclo ./Middlewares/uTKernel/sysdepend/stm32_cube/hw_setting.d ./Middlewares/uTKernel/sysdepend/stm32_cube/hw_setting.o ./Middlewares/uTKernel/sysdepend/stm32_cube/hw_setting.su ./Middlewares/uTKernel/sysdepend/stm32_cube/power_save.cyclo ./Middlewares/uTKernel/sysdepend/stm32_cube/power_save.d ./Middlewares/uTKernel/sysdepend/stm32_cube/power_save.o ./Middlewares/uTKernel/sysdepend/stm32_cube/power_save.su

.PHONY: clean-Middlewares-2f-uTKernel-2f-sysdepend-2f-stm32_cube

