################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/cortexm/_initialize_hardware.c \
../system/src/cortexm/_reset_hardware.c \
../system/src/cortexm/exception_handlers.c 

OBJS += \
./system/src/cortexm/_initialize_hardware.o \
./system/src/cortexm/_reset_hardware.o \
./system/src/cortexm/exception_handlers.o 

C_DEPS += \
./system/src/cortexm/_initialize_hardware.d \
./system/src/cortexm/_reset_hardware.d \
./system/src/cortexm/exception_handlers.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/cortexm/%.o: ../system/src/cortexm/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mno-unaligned-access -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DTRACE -DOS_USE_TRACE_ITM -DDISABLE_BLE -DNRF52832_XXAA -DNRF52 -DCUSTOM_BRD -DARM_MATH_CM4 -DCLOCK_ENABLED -I"../system/include" -I"../system/include/cmsis" -I"../system/include/DEVICE" -I"../config" -I"../inc" -I"../inc_c" -I"../include" -I"../src" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_radio_notification" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_bas" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_dfu" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_dis" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_hids" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/common" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/device" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/ble_flash" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/delay" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/hal" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/nrf_soc_nosd" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/pdm" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/twi_master" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/gpiote" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/libraries/scheduler" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/libraries/timer" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/libraries/util" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/softdevice/common/softdevice_handler" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/softdevice/s132/headers" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


