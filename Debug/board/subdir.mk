################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/board.c \
../board/clock_config.c \
../board/dcd.c \
../board/peripherals.c \
../board/pin_mux.c 

OBJS += \
./board/board.o \
./board/clock_config.o \
./board/dcd.o \
./board/peripherals.o \
./board/pin_mux.o 

C_DEPS += \
./board/board.d \
./board/clock_config.d \
./board/dcd.d \
./board/peripherals.d \
./board/pin_mux.d 


# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MIMXRT1062DVL6A -DCPU_MIMXRT1062DVL6A_cm7 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DFSL_RTOS_FREE_RTOS -DSDK_OS_FREE_RTOS -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"D:\Documentos HDD\MCUxpresso\VSG_control\freertos\freertos_kernel\include" -I"D:\Documentos HDD\MCUxpresso\VSG_control\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"D:\Documentos HDD\MCUxpresso\VSG_control\drivers" -I"D:\Documentos HDD\MCUxpresso\VSG_control\xip" -I"D:\Documentos HDD\MCUxpresso\VSG_control\component\serial_manager" -I"D:\Documentos HDD\MCUxpresso\VSG_control\component\uart" -I"D:\Documentos HDD\MCUxpresso\VSG_control\utilities" -I"D:\Documentos HDD\MCUxpresso\VSG_control\freemaster" -I"D:\Documentos HDD\MCUxpresso\VSG_control\component\lists" -I"D:\Documentos HDD\MCUxpresso\VSG_control\source" -I"D:\Documentos HDD\MCUxpresso\VSG_control\freertos\template\ARM_CM4F" -I"D:\Documentos HDD\MCUxpresso\VSG_control\CMSIS" -I"D:\Documentos HDD\MCUxpresso\VSG_control\device" -I"D:\Documentos HDD\MCUxpresso\VSG_control\freertos\freertos_kernel\include" -I"D:\Documentos HDD\MCUxpresso\VSG_control\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"D:\Documentos HDD\MCUxpresso\VSG_control\drivers" -I"D:\Documentos HDD\MCUxpresso\VSG_control\xip" -I"D:\Documentos HDD\MCUxpresso\VSG_control\component\serial_manager" -I"D:\Documentos HDD\MCUxpresso\VSG_control\component\uart" -I"D:\Documentos HDD\MCUxpresso\VSG_control\utilities" -I"D:\Documentos HDD\MCUxpresso\VSG_control\freemaster" -I"D:\Documentos HDD\MCUxpresso\VSG_control\component\lists" -I"D:\Documentos HDD\MCUxpresso\VSG_control\source" -I"D:\Documentos HDD\MCUxpresso\VSG_control\freertos\template\ARM_CM4F" -I"D:\Documentos HDD\MCUxpresso\VSG_control\CMSIS" -I"D:\Documentos HDD\MCUxpresso\VSG_control\device" -I"D:\Documentos HDD\MCUxpresso\VSG_control\board" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


