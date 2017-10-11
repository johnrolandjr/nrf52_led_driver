################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/devices/fatfs.cpp 

S_UPPER_SRCS += \
../src/devices/NeoCore.S 

OBJS += \
./src/devices/NeoCore.o \
./src/devices/fatfs.o 

S_UPPER_DEPS += \
./src/devices/NeoCore.d 

CPP_DEPS += \
./src/devices/fatfs.d 


# Each subdirectory must supply rules for building sources it contributes
src/devices/%.o: ../src/devices/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mno-unaligned-access -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -x assembler-with-cpp -DDISABLE_BLE -DCUSTOM_BRD -DCLOCK_ENABLED -DARM_MATH_CM4 -I"../config" -I"../inc" -I"../inc_c" -I"../src" -I"C:\Users\johnr\dev\external\CMSIS_5\CMSIS\Core\Include" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_radio_notification" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_bas" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_dfu" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_dis" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_hids" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/common" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/device" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/ble_flash" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/delay" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/hal" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/nrf_soc_nosd" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/pdm" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/twi_master" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/gpiote" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/libraries/scheduler" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/libraries/timer" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/libraries/util" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/softdevice/common/softdevice_handler" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/softdevice/s132/headers" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/devices/%.o: ../src/devices/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mno-unaligned-access -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDISABLE_BLE -DNRF52832_XXAA -DNRF52 -DCUSTOM_BRD -DCLOCK_ENABLED -DARM_MATH_CM4 -I"../config" -I"../inc" -I"../inc_c" -I"../src" -I"C:\Users\johnr\dev\external\CMSIS_5\CMSIS\Core\Include" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_radio_notification" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_bas" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_dfu" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_dis" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_hids" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/common" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/device" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/ble_flash" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/delay" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/hal" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/nrf_soc_nosd" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/pdm" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/twi_master" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/gpiote" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/libraries/scheduler" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/libraries/timer" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/libraries/util" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/softdevice/common/softdevice_handler" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/softdevice/s132/headers" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


