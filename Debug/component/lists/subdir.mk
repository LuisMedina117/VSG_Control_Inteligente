################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/lists/fsl_component_generic_list.c 

OBJS += \
./component/lists/fsl_component_generic_list.o 

C_DEPS += \
./component/lists/fsl_component_generic_list.d 


# Each subdirectory must supply rules for building sources it contributes
component/lists/%.o: ../component/lists/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MIMXRT1062DVL6A -DCPU_MIMXRT1062DVL6A_cm7 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DFSL_RTOS_FREE_RTOS -DSDK_OS_FREE_RTOS -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\Luis Daniel Medina\Documents\GIT\VSG_Control\freertos\freertos_kernel\include" -I"C:\Users\Luis Daniel Medina\Documents\GIT\VSG_Control\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"C:\Users\Luis Daniel Medina\Documents\GIT\VSG_Control\drivers" -I"C:\Users\Luis Daniel Medina\Documents\GIT\VSG_Control\xip" -I"C:\Users\Luis Daniel Medina\Documents\GIT\VSG_Control\component\serial_manager" -I"C:\Users\Luis Daniel Medina\Documents\GIT\VSG_Control\component\uart" -I"C:\Users\Luis Daniel Medina\Documents\GIT\VSG_Control\utilities" -I"C:\Users\Luis Daniel Medina\Documents\GIT\VSG_Control\freemaster" -I"C:\Users\Luis Daniel Medina\Documents\GIT\VSG_Control\component\lists" -I"C:\Users\Luis Daniel Medina\Documents\GIT\VSG_Control\source" -I"C:\Users\Luis Daniel Medina\Documents\GIT\VSG_Control\CMSIS" -I"C:\Users\Luis Daniel Medina\Documents\GIT\VSG_Control\device" -I"C:\Users\Luis Daniel Medina\Documents\GIT\VSG_Control\freertos\freertos_kernel\include" -I"C:\Users\Luis Daniel Medina\Documents\GIT\VSG_Control\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"C:\Users\Luis Daniel Medina\Documents\GIT\VSG_Control\drivers" -I"C:\Users\Luis Daniel Medina\Documents\GIT\VSG_Control\xip" -I"C:\Users\Luis Daniel Medina\Documents\GIT\VSG_Control\component\serial_manager" -I"C:\Users\Luis Daniel Medina\Documents\GIT\VSG_Control\component\uart" -I"C:\Users\Luis Daniel Medina\Documents\GIT\VSG_Control\utilities" -I"C:\Users\Luis Daniel Medina\Documents\GIT\VSG_Control\freemaster" -I"C:\Users\Luis Daniel Medina\Documents\GIT\VSG_Control\component\lists" -I"C:\Users\Luis Daniel Medina\Documents\GIT\VSG_Control\source" -I"C:\Users\Luis Daniel Medina\Documents\GIT\VSG_Control\CMSIS" -I"C:\Users\Luis Daniel Medina\Documents\GIT\VSG_Control\device" -I"C:\Users\Luis Daniel Medina\Documents\GIT\VSG_Control\board" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


