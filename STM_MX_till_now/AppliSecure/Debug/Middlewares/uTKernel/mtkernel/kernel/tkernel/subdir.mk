################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/uTKernel/mtkernel/kernel/tkernel/cpuctl.c \
../Middlewares/uTKernel/mtkernel/kernel/tkernel/device.c \
../Middlewares/uTKernel/mtkernel/kernel/tkernel/deviceio.c \
../Middlewares/uTKernel/mtkernel/kernel/tkernel/eventflag.c \
../Middlewares/uTKernel/mtkernel/kernel/tkernel/int.c \
../Middlewares/uTKernel/mtkernel/kernel/tkernel/klock.c \
../Middlewares/uTKernel/mtkernel/kernel/tkernel/mailbox.c \
../Middlewares/uTKernel/mtkernel/kernel/tkernel/memory.c \
../Middlewares/uTKernel/mtkernel/kernel/tkernel/mempfix.c \
../Middlewares/uTKernel/mtkernel/kernel/tkernel/mempool.c \
../Middlewares/uTKernel/mtkernel/kernel/tkernel/messagebuf.c \
../Middlewares/uTKernel/mtkernel/kernel/tkernel/misc_calls.c \
../Middlewares/uTKernel/mtkernel/kernel/tkernel/mutex.c \
../Middlewares/uTKernel/mtkernel/kernel/tkernel/objname.c \
../Middlewares/uTKernel/mtkernel/kernel/tkernel/power.c \
../Middlewares/uTKernel/mtkernel/kernel/tkernel/rendezvous.c \
../Middlewares/uTKernel/mtkernel/kernel/tkernel/semaphore.c \
../Middlewares/uTKernel/mtkernel/kernel/tkernel/task.c \
../Middlewares/uTKernel/mtkernel/kernel/tkernel/task_manage.c \
../Middlewares/uTKernel/mtkernel/kernel/tkernel/task_sync.c \
../Middlewares/uTKernel/mtkernel/kernel/tkernel/time_calls.c \
../Middlewares/uTKernel/mtkernel/kernel/tkernel/timer.c \
../Middlewares/uTKernel/mtkernel/kernel/tkernel/tkinit.c \
../Middlewares/uTKernel/mtkernel/kernel/tkernel/wait.c 

OBJS += \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/cpuctl.o \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/device.o \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/deviceio.o \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/eventflag.o \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/int.o \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/klock.o \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/mailbox.o \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/memory.o \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/mempfix.o \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/mempool.o \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/messagebuf.o \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/misc_calls.o \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/mutex.o \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/objname.o \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/power.o \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/rendezvous.o \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/semaphore.o \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/task.o \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/task_manage.o \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/task_sync.o \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/time_calls.o \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/timer.o \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/tkinit.o \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/wait.o 

C_DEPS += \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/cpuctl.d \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/device.d \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/deviceio.d \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/eventflag.d \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/int.d \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/klock.d \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/mailbox.d \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/memory.d \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/mempfix.d \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/mempool.d \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/messagebuf.d \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/misc_calls.d \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/mutex.d \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/objname.d \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/power.d \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/rendezvous.d \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/semaphore.d \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/task.d \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/task_manage.d \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/task_sync.d \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/time_calls.d \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/timer.d \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/tkinit.d \
./Middlewares/uTKernel/mtkernel/kernel/tkernel/wait.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/uTKernel/mtkernel/kernel/tkernel/%.o Middlewares/uTKernel/mtkernel/kernel/tkernel/%.su Middlewares/uTKernel/mtkernel/kernel/tkernel/%.cyclo: ../Middlewares/uTKernel/mtkernel/kernel/tkernel/%.c Middlewares/uTKernel/mtkernel/kernel/tkernel/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m55 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32N657xx -D_STM32CUBE_DISCOVERY_N657_ -DCNF_SYSTEMAREA_END=0x34100000 -DLL_ATON_DUMP_DEBUG_API -DLL_ATON_PLATFORM=LL_ATON_PLAT_STM32N6 -DLL_ATON_OSAL=LL_ATON_OSAL_BARE_METAL -DLL_ATON_RT_MODE=LL_ATON_RT_ASYNC -DLL_ATON_SW_FALLBACK -DLL_ATON_EB_DBG_INFO -DLL_ATON_DBG_BUFFER_INFO_EXCLUDED=1 -c -I../Core/Inc -I"C:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel/mtkernel/kernel/knlinc" -I../../Secure_nsclib -I../../Middlewares/ST/AI/Npu/Devices/STM32N6XX -I../../Middlewares/ST/AI/Inc -I../../Middlewares/ST/AI/Npu/ll_aton -I../X-CUBE-AI/App -I../../Drivers/STM32N6xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32N6xx/Include -I../../Drivers/STM32N6xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Include -I../../AppliSecure/X-CUBE-AI/App -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel/include -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel/config -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Core/Inc -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Core/Inc -IC:/Users/user/Documents/Sentinel-RT/STM_MX_till_now/AppliSecure/Middlewares/uTKernel/include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -mcmse -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-uTKernel-2f-mtkernel-2f-kernel-2f-tkernel

clean-Middlewares-2f-uTKernel-2f-mtkernel-2f-kernel-2f-tkernel:
	-$(RM) ./Middlewares/uTKernel/mtkernel/kernel/tkernel/cpuctl.cyclo ./Middlewares/uTKernel/mtkernel/kernel/tkernel/cpuctl.d ./Middlewares/uTKernel/mtkernel/kernel/tkernel/cpuctl.o ./Middlewares/uTKernel/mtkernel/kernel/tkernel/cpuctl.su ./Middlewares/uTKernel/mtkernel/kernel/tkernel/device.cyclo ./Middlewares/uTKernel/mtkernel/kernel/tkernel/device.d ./Middlewares/uTKernel/mtkernel/kernel/tkernel/device.o ./Middlewares/uTKernel/mtkernel/kernel/tkernel/device.su ./Middlewares/uTKernel/mtkernel/kernel/tkernel/deviceio.cyclo ./Middlewares/uTKernel/mtkernel/kernel/tkernel/deviceio.d ./Middlewares/uTKernel/mtkernel/kernel/tkernel/deviceio.o ./Middlewares/uTKernel/mtkernel/kernel/tkernel/deviceio.su ./Middlewares/uTKernel/mtkernel/kernel/tkernel/eventflag.cyclo ./Middlewares/uTKernel/mtkernel/kernel/tkernel/eventflag.d ./Middlewares/uTKernel/mtkernel/kernel/tkernel/eventflag.o ./Middlewares/uTKernel/mtkernel/kernel/tkernel/eventflag.su ./Middlewares/uTKernel/mtkernel/kernel/tkernel/int.cyclo ./Middlewares/uTKernel/mtkernel/kernel/tkernel/int.d ./Middlewares/uTKernel/mtkernel/kernel/tkernel/int.o ./Middlewares/uTKernel/mtkernel/kernel/tkernel/int.su ./Middlewares/uTKernel/mtkernel/kernel/tkernel/klock.cyclo ./Middlewares/uTKernel/mtkernel/kernel/tkernel/klock.d ./Middlewares/uTKernel/mtkernel/kernel/tkernel/klock.o ./Middlewares/uTKernel/mtkernel/kernel/tkernel/klock.su ./Middlewares/uTKernel/mtkernel/kernel/tkernel/mailbox.cyclo ./Middlewares/uTKernel/mtkernel/kernel/tkernel/mailbox.d ./Middlewares/uTKernel/mtkernel/kernel/tkernel/mailbox.o ./Middlewares/uTKernel/mtkernel/kernel/tkernel/mailbox.su ./Middlewares/uTKernel/mtkernel/kernel/tkernel/memory.cyclo ./Middlewares/uTKernel/mtkernel/kernel/tkernel/memory.d ./Middlewares/uTKernel/mtkernel/kernel/tkernel/memory.o ./Middlewares/uTKernel/mtkernel/kernel/tkernel/memory.su ./Middlewares/uTKernel/mtkernel/kernel/tkernel/mempfix.cyclo ./Middlewares/uTKernel/mtkernel/kernel/tkernel/mempfix.d ./Middlewares/uTKernel/mtkernel/kernel/tkernel/mempfix.o ./Middlewares/uTKernel/mtkernel/kernel/tkernel/mempfix.su ./Middlewares/uTKernel/mtkernel/kernel/tkernel/mempool.cyclo ./Middlewares/uTKernel/mtkernel/kernel/tkernel/mempool.d ./Middlewares/uTKernel/mtkernel/kernel/tkernel/mempool.o ./Middlewares/uTKernel/mtkernel/kernel/tkernel/mempool.su ./Middlewares/uTKernel/mtkernel/kernel/tkernel/messagebuf.cyclo ./Middlewares/uTKernel/mtkernel/kernel/tkernel/messagebuf.d ./Middlewares/uTKernel/mtkernel/kernel/tkernel/messagebuf.o ./Middlewares/uTKernel/mtkernel/kernel/tkernel/messagebuf.su ./Middlewares/uTKernel/mtkernel/kernel/tkernel/misc_calls.cyclo ./Middlewares/uTKernel/mtkernel/kernel/tkernel/misc_calls.d ./Middlewares/uTKernel/mtkernel/kernel/tkernel/misc_calls.o ./Middlewares/uTKernel/mtkernel/kernel/tkernel/misc_calls.su ./Middlewares/uTKernel/mtkernel/kernel/tkernel/mutex.cyclo ./Middlewares/uTKernel/mtkernel/kernel/tkernel/mutex.d ./Middlewares/uTKernel/mtkernel/kernel/tkernel/mutex.o ./Middlewares/uTKernel/mtkernel/kernel/tkernel/mutex.su ./Middlewares/uTKernel/mtkernel/kernel/tkernel/objname.cyclo ./Middlewares/uTKernel/mtkernel/kernel/tkernel/objname.d ./Middlewares/uTKernel/mtkernel/kernel/tkernel/objname.o ./Middlewares/uTKernel/mtkernel/kernel/tkernel/objname.su ./Middlewares/uTKernel/mtkernel/kernel/tkernel/power.cyclo ./Middlewares/uTKernel/mtkernel/kernel/tkernel/power.d ./Middlewares/uTKernel/mtkernel/kernel/tkernel/power.o ./Middlewares/uTKernel/mtkernel/kernel/tkernel/power.su ./Middlewares/uTKernel/mtkernel/kernel/tkernel/rendezvous.cyclo ./Middlewares/uTKernel/mtkernel/kernel/tkernel/rendezvous.d ./Middlewares/uTKernel/mtkernel/kernel/tkernel/rendezvous.o ./Middlewares/uTKernel/mtkernel/kernel/tkernel/rendezvous.su ./Middlewares/uTKernel/mtkernel/kernel/tkernel/semaphore.cyclo ./Middlewares/uTKernel/mtkernel/kernel/tkernel/semaphore.d ./Middlewares/uTKernel/mtkernel/kernel/tkernel/semaphore.o ./Middlewares/uTKernel/mtkernel/kernel/tkernel/semaphore.su ./Middlewares/uTKernel/mtkernel/kernel/tkernel/task.cyclo ./Middlewares/uTKernel/mtkernel/kernel/tkernel/task.d ./Middlewares/uTKernel/mtkernel/kernel/tkernel/task.o ./Middlewares/uTKernel/mtkernel/kernel/tkernel/task.su ./Middlewares/uTKernel/mtkernel/kernel/tkernel/task_manage.cyclo ./Middlewares/uTKernel/mtkernel/kernel/tkernel/task_manage.d ./Middlewares/uTKernel/mtkernel/kernel/tkernel/task_manage.o ./Middlewares/uTKernel/mtkernel/kernel/tkernel/task_manage.su ./Middlewares/uTKernel/mtkernel/kernel/tkernel/task_sync.cyclo ./Middlewares/uTKernel/mtkernel/kernel/tkernel/task_sync.d ./Middlewares/uTKernel/mtkernel/kernel/tkernel/task_sync.o ./Middlewares/uTKernel/mtkernel/kernel/tkernel/task_sync.su ./Middlewares/uTKernel/mtkernel/kernel/tkernel/time_calls.cyclo ./Middlewares/uTKernel/mtkernel/kernel/tkernel/time_calls.d ./Middlewares/uTKernel/mtkernel/kernel/tkernel/time_calls.o ./Middlewares/uTKernel/mtkernel/kernel/tkernel/time_calls.su ./Middlewares/uTKernel/mtkernel/kernel/tkernel/timer.cyclo ./Middlewares/uTKernel/mtkernel/kernel/tkernel/timer.d ./Middlewares/uTKernel/mtkernel/kernel/tkernel/timer.o ./Middlewares/uTKernel/mtkernel/kernel/tkernel/timer.su ./Middlewares/uTKernel/mtkernel/kernel/tkernel/tkinit.cyclo ./Middlewares/uTKernel/mtkernel/kernel/tkernel/tkinit.d ./Middlewares/uTKernel/mtkernel/kernel/tkernel/tkinit.o ./Middlewares/uTKernel/mtkernel/kernel/tkernel/tkinit.su ./Middlewares/uTKernel/mtkernel/kernel/tkernel/wait.cyclo ./Middlewares/uTKernel/mtkernel/kernel/tkernel/wait.d ./Middlewares/uTKernel/mtkernel/kernel/tkernel/wait.o ./Middlewares/uTKernel/mtkernel/kernel/tkernel/wait.su

.PHONY: clean-Middlewares-2f-uTKernel-2f-mtkernel-2f-kernel-2f-tkernel

