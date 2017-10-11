################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/nrf/app_error.c \
../src/nrf/app_error_weak.c \
../src/nrf/app_gpiote.c \
../src/nrf/app_sdcard.c \
../src/nrf/app_timer.c \
../src/nrf/app_util_platform.c \
../src/nrf/diskio.c \
../src/nrf/ff.c \
../src/nrf/i2c_api.c \
../src/nrf/nrf_drv_clock.c \
../src/nrf/nrf_drv_common.c \
../src/nrf/nrf_drv_gpiote.c \
../src/nrf/nrf_drv_pdm.c \
../src/nrf/nrf_drv_uart.c \
../src/nrf/softdevice_handler.c \
../src/nrf/system_nrf52.c 

S_UPPER_SRCS += \
../src/nrf/gcc_startup_nrf52.S 

OBJS += \
./src/nrf/app_error.o \
./src/nrf/app_error_weak.o \
./src/nrf/app_gpiote.o \
./src/nrf/app_sdcard.o \
./src/nrf/app_timer.o \
./src/nrf/app_util_platform.o \
./src/nrf/diskio.o \
./src/nrf/ff.o \
./src/nrf/gcc_startup_nrf52.o \
./src/nrf/i2c_api.o \
./src/nrf/nrf_drv_clock.o \
./src/nrf/nrf_drv_common.o \
./src/nrf/nrf_drv_gpiote.o \
./src/nrf/nrf_drv_pdm.o \
./src/nrf/nrf_drv_uart.o \
./src/nrf/softdevice_handler.o \
./src/nrf/system_nrf52.o 

S_UPPER_DEPS += \
./src/nrf/gcc_startup_nrf52.d 

C_DEPS += \
./src/nrf/app_error.d \
./src/nrf/app_error_weak.d \
./src/nrf/app_gpiote.d \
./src/nrf/app_sdcard.d \
./src/nrf/app_timer.d \
./src/nrf/app_util_platform.d \
./src/nrf/diskio.d \
./src/nrf/ff.d \
./src/nrf/i2c_api.d \
./src/nrf/nrf_drv_clock.d \
./src/nrf/nrf_drv_common.d \
./src/nrf/nrf_drv_gpiote.d \
./src/nrf/nrf_drv_pdm.d \
./src/nrf/nrf_drv_uart.d \
./src/nrf/softdevice_handler.d \
./src/nrf/system_nrf52.d 


# Each subdirectory must supply rules for building sources it contributes
src/nrf/%.o: ../src/nrf/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mno-unaligned-access -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDISABLE_BLE -DNRF52832_XXAA -DNRF52 -DCUSTOM_BRD -DCLOCK_ENABLED -DARM_MATH_CM4 -I"../config" -I"../inc" -I"../inc_c" -I"../src" -I"C:\Users\johnr\dev\external\CMSIS_5\CMSIS\Core\Include" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_radio_notification" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_bas" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_dfu" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_dis" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_hids" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/common" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/device" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/ble_flash" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/delay" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/hal" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/nrf_soc_nosd" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/pdm" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/twi_master" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/gpiote" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/libraries/scheduler" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/libraries/timer" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/libraries/util" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/softdevice/common/softdevice_handler" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/softdevice/s132/headers" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/nrf/%.o: ../src/nrf/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mno-unaligned-access -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -x assembler-with-cpp -DDISABLE_BLE -DCUSTOM_BRD -DCLOCK_ENABLED -DARM_MATH_CM4 -I"../config" -I"../inc" -I"../inc_c" -I"../src" -I"C:\Users\johnr\dev\external\CMSIS_5\CMSIS\Core\Include" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_radio_notification" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_bas" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_dfu" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_dis" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/ble_services/ble_hids" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/ble/common" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/device" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/ble_flash" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/delay" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/hal" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/nrf_soc_nosd" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/pdm" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/twi_master" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/drivers_nrf/gpiote" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/libraries/scheduler" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/libraries/timer" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/libraries/util" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/softdevice/common/softdevice_handler" -I"C:\Users\johnr\dev\workspace/../external/nrf5_sdk/components/softdevice/s132/headers" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


