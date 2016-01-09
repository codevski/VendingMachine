################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bakup/ppd_coin.c \
../bakup/ppd_main.c \
../bakup/ppd_menu.c \
../bakup/ppd_options.c \
../bakup/ppd_stock.c \
../bakup/ppd_utility.c 

OBJS += \
./bakup/ppd_coin.o \
./bakup/ppd_main.o \
./bakup/ppd_menu.o \
./bakup/ppd_options.o \
./bakup/ppd_stock.o \
./bakup/ppd_utility.o 

C_DEPS += \
./bakup/ppd_coin.d \
./bakup/ppd_main.d \
./bakup/ppd_menu.d \
./bakup/ppd_options.d \
./bakup/ppd_stock.d \
./bakup/ppd_utility.d 


# Each subdirectory must supply rules for building sources it contributes
bakup/%.o: ../bakup/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


