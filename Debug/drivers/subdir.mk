################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_adc.c \
../drivers/fsl_adc_etc.c \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_flexio.c \
../drivers/fsl_flexio_uart.c \
../drivers/fsl_gpio.c \
../drivers/fsl_lpuart.c \
../drivers/fsl_pwm.c \
../drivers/fsl_xbara.c 

OBJS += \
./drivers/fsl_adc.o \
./drivers/fsl_adc_etc.o \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_flexio.o \
./drivers/fsl_flexio_uart.o \
./drivers/fsl_gpio.o \
./drivers/fsl_lpuart.o \
./drivers/fsl_pwm.o \
./drivers/fsl_xbara.o 

C_DEPS += \
./drivers/fsl_adc.d \
./drivers/fsl_adc_etc.d \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_flexio.d \
./drivers/fsl_flexio_uart.d \
./drivers/fsl_gpio.d \
./drivers/fsl_lpuart.d \
./drivers/fsl_pwm.d \
./drivers/fsl_xbara.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MIMXRT1062DVL6A -DCPU_MIMXRT1062DVL6A_cm7 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DFSL_RTOS_FREE_RTOS -DSDK_OS_FREE_RTOS -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"D:\Documentos HDD\GIT\VSG_Control\freertos\freertos_kernel\include" -I"D:\Documentos HDD\GIT\VSG_Control\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"D:\Documentos HDD\GIT\VSG_Control\drivers" -I"D:\Documentos HDD\GIT\VSG_Control\xip" -I"D:\Documentos HDD\GIT\VSG_Control\component\serial_manager" -I"D:\Documentos HDD\GIT\VSG_Control\component\uart" -I"D:\Documentos HDD\GIT\VSG_Control\utilities" -I"D:\Documentos HDD\GIT\VSG_Control\freemaster" -I"D:\Documentos HDD\GIT\VSG_Control\component\lists" -I"D:\Documentos HDD\GIT\VSG_Control\source" -I"D:\Documentos HDD\GIT\VSG_Control\CMSIS" -I"D:\Documentos HDD\GIT\VSG_Control\device" -I"D:\Documentos HDD\GIT\VSG_Control\freertos\freertos_kernel\include" -I"D:\Documentos HDD\GIT\VSG_Control\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"D:\Documentos HDD\GIT\VSG_Control\drivers" -I"D:\Documentos HDD\GIT\VSG_Control\xip" -I"D:\Documentos HDD\GIT\VSG_Control\component\serial_manager" -I"D:\Documentos HDD\GIT\VSG_Control\component\uart" -I"D:\Documentos HDD\GIT\VSG_Control\utilities" -I"D:\Documentos HDD\GIT\VSG_Control\freemaster" -I"D:\Documentos HDD\GIT\VSG_Control\component\lists" -I"D:\Documentos HDD\GIT\VSG_Control\source" -I"D:\Documentos HDD\GIT\VSG_Control\CMSIS" -I"D:\Documentos HDD\GIT\VSG_Control\device" -I"D:\Documentos HDD\GIT\VSG_Control\board" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


