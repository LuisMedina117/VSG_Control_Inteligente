################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/ADCbib.c \
../source/Control.c \
../source/FreeMasterbib.c \
../source/GPIObib.c \
../source/PWMbib.c \
../source/Principal.c \
../source/ProMed.c \
../source/TablasBusqueda.c \
../source/XBARbib.c \
../source/semihost_hardfault.c 

OBJS += \
./source/ADCbib.o \
./source/Control.o \
./source/FreeMasterbib.o \
./source/GPIObib.o \
./source/PWMbib.o \
./source/Principal.o \
./source/ProMed.o \
./source/TablasBusqueda.o \
./source/XBARbib.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/ADCbib.d \
./source/Control.d \
./source/FreeMasterbib.d \
./source/GPIObib.d \
./source/PWMbib.d \
./source/Principal.d \
./source/ProMed.d \
./source/TablasBusqueda.d \
./source/XBARbib.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MIMXRT1062DVL6A -DCPU_MIMXRT1062DVL6A_cm7 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DFSL_RTOS_FREE_RTOS -DSDK_OS_FREE_RTOS -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"D:\Documentos HDD\GIT\VSG_Control\freertos\freertos_kernel\include" -I"D:\Documentos HDD\GIT\VSG_Control\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"D:\Documentos HDD\GIT\VSG_Control\drivers" -I"D:\Documentos HDD\GIT\VSG_Control\xip" -I"D:\Documentos HDD\GIT\VSG_Control\component\serial_manager" -I"D:\Documentos HDD\GIT\VSG_Control\component\uart" -I"D:\Documentos HDD\GIT\VSG_Control\utilities" -I"D:\Documentos HDD\GIT\VSG_Control\freemaster" -I"D:\Documentos HDD\GIT\VSG_Control\component\lists" -I"D:\Documentos HDD\GIT\VSG_Control\source" -I"D:\Documentos HDD\GIT\VSG_Control\freertos\template\ARM_CM4F" -I"D:\Documentos HDD\GIT\VSG_Control\CMSIS" -I"D:\Documentos HDD\GIT\VSG_Control\device" -I"D:\Documentos HDD\GIT\VSG_Control\freertos\freertos_kernel\include" -I"D:\Documentos HDD\GIT\VSG_Control\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"D:\Documentos HDD\GIT\VSG_Control\drivers" -I"D:\Documentos HDD\GIT\VSG_Control\xip" -I"D:\Documentos HDD\GIT\VSG_Control\component\serial_manager" -I"D:\Documentos HDD\GIT\VSG_Control\component\uart" -I"D:\Documentos HDD\GIT\VSG_Control\utilities" -I"D:\Documentos HDD\GIT\VSG_Control\freemaster" -I"D:\Documentos HDD\GIT\VSG_Control\component\lists" -I"D:\Documentos HDD\GIT\VSG_Control\source" -I"D:\Documentos HDD\GIT\VSG_Control\freertos\template\ARM_CM4F" -I"D:\Documentos HDD\GIT\VSG_Control\CMSIS" -I"D:\Documentos HDD\GIT\VSG_Control\device" -I"D:\Documentos HDD\GIT\VSG_Control\board" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


