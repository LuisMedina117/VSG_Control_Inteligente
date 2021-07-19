################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freertos/freertos_kernel/croutine.c \
../freertos/freertos_kernel/event_groups.c \
../freertos/freertos_kernel/list.c \
../freertos/freertos_kernel/queue.c \
../freertos/freertos_kernel/stream_buffer.c \
../freertos/freertos_kernel/tasks.c \
../freertos/freertos_kernel/timers.c 

OBJS += \
./freertos/freertos_kernel/croutine.o \
./freertos/freertos_kernel/event_groups.o \
./freertos/freertos_kernel/list.o \
./freertos/freertos_kernel/queue.o \
./freertos/freertos_kernel/stream_buffer.o \
./freertos/freertos_kernel/tasks.o \
./freertos/freertos_kernel/timers.o 

C_DEPS += \
./freertos/freertos_kernel/croutine.d \
./freertos/freertos_kernel/event_groups.d \
./freertos/freertos_kernel/list.d \
./freertos/freertos_kernel/queue.d \
./freertos/freertos_kernel/stream_buffer.d \
./freertos/freertos_kernel/tasks.d \
./freertos/freertos_kernel/timers.d 


# Each subdirectory must supply rules for building sources it contributes
freertos/freertos_kernel/%.o: ../freertos/freertos_kernel/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MIMXRT1062DVL6A -DCPU_MIMXRT1062DVL6A_cm7 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DFSL_RTOS_FREE_RTOS -DSDK_OS_FREE_RTOS -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"D:\Documentos HDD\GIT\VSG_Control\freertos\freertos_kernel\include" -I"D:\Documentos HDD\GIT\VSG_Control\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"D:\Documentos HDD\GIT\VSG_Control\drivers" -I"D:\Documentos HDD\GIT\VSG_Control\xip" -I"D:\Documentos HDD\GIT\VSG_Control\component\serial_manager" -I"D:\Documentos HDD\GIT\VSG_Control\component\uart" -I"D:\Documentos HDD\GIT\VSG_Control\utilities" -I"D:\Documentos HDD\GIT\VSG_Control\freemaster" -I"D:\Documentos HDD\GIT\VSG_Control\component\lists" -I"D:\Documentos HDD\GIT\VSG_Control\source" -I"D:\Documentos HDD\GIT\VSG_Control\freertos\template\ARM_CM4F" -I"D:\Documentos HDD\GIT\VSG_Control\CMSIS" -I"D:\Documentos HDD\GIT\VSG_Control\device" -I"D:\Documentos HDD\GIT\VSG_Control\freertos\freertos_kernel\include" -I"D:\Documentos HDD\GIT\VSG_Control\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"D:\Documentos HDD\GIT\VSG_Control\drivers" -I"D:\Documentos HDD\GIT\VSG_Control\xip" -I"D:\Documentos HDD\GIT\VSG_Control\component\serial_manager" -I"D:\Documentos HDD\GIT\VSG_Control\component\uart" -I"D:\Documentos HDD\GIT\VSG_Control\utilities" -I"D:\Documentos HDD\GIT\VSG_Control\freemaster" -I"D:\Documentos HDD\GIT\VSG_Control\component\lists" -I"D:\Documentos HDD\GIT\VSG_Control\source" -I"D:\Documentos HDD\GIT\VSG_Control\freertos\template\ARM_CM4F" -I"D:\Documentos HDD\GIT\VSG_Control\CMSIS" -I"D:\Documentos HDD\GIT\VSG_Control\device" -I"D:\Documentos HDD\GIT\VSG_Control\board" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


