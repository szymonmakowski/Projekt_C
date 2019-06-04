################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/SendUDP.c \
../src/ipv4.c \
../src/lista.c \
../src/sendudp_functions.c \
../src/udp.c 

OBJS += \
./src/SendUDP.o \
./src/ipv4.o \
./src/lista.o \
./src/sendudp_functions.o \
./src/udp.o 

C_DEPS += \
./src/SendUDP.d \
./src/ipv4.d \
./src/lista.d \
./src/sendudp_functions.d \
./src/udp.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


