################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/newlib/_exit.c \
../system/src/newlib/_sbrk.c \
../system/src/newlib/_startup.c \
../system/src/newlib/_syscalls.c \
../system/src/newlib/assert.c 

CPP_SRCS += \
../system/src/newlib/_cxx.cpp 

OBJS += \
./system/src/newlib/_cxx.o \
./system/src/newlib/_exit.o \
./system/src/newlib/_sbrk.o \
./system/src/newlib/_startup.o \
./system/src/newlib/_syscalls.o \
./system/src/newlib/assert.o 

C_DEPS += \
./system/src/newlib/_exit.d \
./system/src/newlib/_sbrk.d \
./system/src/newlib/_startup.d \
./system/src/newlib/_syscalls.d \
./system/src/newlib/assert.d 

CPP_DEPS += \
./system/src/newlib/_cxx.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/newlib/%.o: ../system/src/newlib/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mno-unaligned-access -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DTRACE -DOS_USE_TRACE_ITM -DDISABLE_BLE -DNRF52832_XXAA -DNRF52 -DCUSTOM_BRD -DARM_MATH_CM4 -DCLOCK_ENABLED -I"../system/include" -I"../system/include/cmsis" -I"../system/include/DEVICE" -I"../config" -I"../inc" -I"../inc_c" -I"../include" -I"../src" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_radio_notification" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_bas" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_dfu" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_dis" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_hids" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/common" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/device" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/ble_flash" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/delay" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/hal" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/nrf_soc_nosd" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/pdm" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/twi_master" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/gpiote" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/libraries/scheduler" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/libraries/timer" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/libraries/util" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/softdevice/common/softdevice_handler" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/softdevice/s132/headers" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/src/newlib/%.o: ../system/src/newlib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mno-unaligned-access -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DTRACE -DOS_USE_TRACE_ITM -DDISABLE_BLE -DNRF52832_XXAA -DNRF52 -DCUSTOM_BRD -DARM_MATH_CM4 -DCLOCK_ENABLED -I"../system/include" -I"../system/include/cmsis" -I"../system/include/DEVICE" -I"../config" -I"../inc" -I"../inc_c" -I"../include" -I"../src" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_radio_notification" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_bas" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_dfu" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_dis" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_hids" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/common" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/device" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/ble_flash" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/delay" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/hal" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/nrf_soc_nosd" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/pdm" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/twi_master" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/gpiote" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/libraries/scheduler" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/libraries/timer" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/libraries/util" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/softdevice/common/softdevice_handler" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/softdevice/s132/headers" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


