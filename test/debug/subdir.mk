################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../ppd_coin.o \
../ppd_main.o \
../ppd_menu.o \
../ppd_options.o \
../ppd_stock.o \
../ppd_utility.o 

C_SRCS += \
../ppd_coin.c \
../ppd_main.c \
../ppd_menu.c \
../ppd_options.c \
../ppd_stock.c \
../ppd_utility.c 

OBJS += \
./ppd_coin.o \
./ppd_main.o \
./ppd_menu.o \
./ppd_options.o \
./ppd_stock.o \
./ppd_utility.o 

C_DEPS += \
./ppd_coin.d \
./ppd_main.d \
./ppd_menu.d \
./ppd_options.d \
./ppd_stock.d \
./ppd_utility.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


