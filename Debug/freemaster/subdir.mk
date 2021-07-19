################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freemaster/freemaster_appcmd.c \
../freemaster/freemaster_can.c \
../freemaster/freemaster_pdbdm.c \
../freemaster/freemaster_pipes.c \
../freemaster/freemaster_protocol.c \
../freemaster/freemaster_rec.c \
../freemaster/freemaster_scope.c \
../freemaster/freemaster_serial.c \
../freemaster/freemaster_serial_lpuart.c \
../freemaster/freemaster_sha.c \
../freemaster/freemaster_tsa.c \
../freemaster/freemaster_ures.c \
../freemaster/freemaster_utils.c 

OBJS += \
./freemaster/freemaster_appcmd.o \
./freemaster/freemaster_can.o \
./freemaster/freemaster_pdbdm.o \
./freemaster/freemaster_pipes.o \
./freemaster/freemaster_protocol.o \
./freemaster/freemaster_rec.o \
./freemaster/freemaster_scope.o \
./freemaster/freemaster_serial.o \
./freemaster/freemaster_serial_lpuart.o \
./freemaster/freemaster_sha.o \
./freemaster/freemaster_tsa.o \
./freemaster/freemaster_ures.o \
./freemaster/freemaster_utils.o 

C_DEPS += \
./freemaster/freemaster_appcmd.d \
./freemaster/freemaster_can.d \
./freemaster/freemaster_pdbdm.d \
./freemaster/freemaster_pipes.d \
./freemaster/freemaster_protocol.d \
./freemaster/freemaster_rec.d \
./freemaster/freemaster_scope.d \
./freemaster/freemaster_serial.d \
./freemaster/freemaster_serial_lpuart.d \
./freemaster/freemaster_sha.d \
./freemaster/freemaster_tsa.d \
./freemaster/freemaster_ures.d \
./freemaster/freemaster_utils.d 


# Each subdirectory must supply rules for building sources it contributes
freemaster/%.o: ../freemaster/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MIMXRT1062DVL6A -DCPU_MIMXRT1062DVL6A_cm7 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DFSL_RTOS_FREE_RTOS -DSDK_OS_FREE_RTOS -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"D:\Documentos HDD\GIT\VSG_Control\freertos\freertos_kernel\include" -I"D:\Documentos HDD\GIT\VSG_Control\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"D:\Documentos HDD\GIT\VSG_Control\drivers" -I"D:\Documentos HDD\GIT\VSG_Control\xip" -I"D:\Documentos HDD\GIT\VSG_Control\component\serial_manager" -I"D:\Documentos HDD\GIT\VSG_Control\component\uart" -I"D:\Documentos HDD\GIT\VSG_Control\utilities" -I"D:\Documentos HDD\GIT\VSG_Control\freemaster" -I"D:\Documentos HDD\GIT\VSG_Control\component\lists" -I"D:\Documentos HDD\GIT\VSG_Control\source" -I"D:\Documentos HDD\GIT\VSG_Control\freertos\template\ARM_CM4F" -I"D:\Documentos HDD\GIT\VSG_Control\CMSIS" -I"D:\Documentos HDD\GIT\VSG_Control\device" -I"D:\Documentos HDD\GIT\VSG_Control\freertos\freertos_kernel\include" -I"D:\Documentos HDD\GIT\VSG_Control\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"D:\Documentos HDD\GIT\VSG_Control\drivers" -I"D:\Documentos HDD\GIT\VSG_Control\xip" -I"D:\Documentos HDD\GIT\VSG_Control\component\serial_manager" -I"D:\Documentos HDD\GIT\VSG_Control\component\uart" -I"D:\Documentos HDD\GIT\VSG_Control\utilities" -I"D:\Documentos HDD\GIT\VSG_Control\freemaster" -I"D:\Documentos HDD\GIT\VSG_Control\component\lists" -I"D:\Documentos HDD\GIT\VSG_Control\source" -I"D:\Documentos HDD\GIT\VSG_Control\freertos\template\ARM_CM4F" -I"D:\Documentos HDD\GIT\VSG_Control\CMSIS" -I"D:\Documentos HDD\GIT\VSG_Control\device" -I"D:\Documentos HDD\GIT\VSG_Control\board" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


