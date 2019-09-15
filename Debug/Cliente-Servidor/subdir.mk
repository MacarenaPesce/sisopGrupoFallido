################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Cliente-Servidor/client.c \
../Cliente-Servidor/server.c 

OBJS += \
./Cliente-Servidor/client.o \
./Cliente-Servidor/server.o 

C_DEPS += \
./Cliente-Servidor/client.d \
./Cliente-Servidor/server.d 


# Each subdirectory must supply rules for building sources it contributes
Cliente-Servidor/%.o: ../Cliente-Servidor/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/Hilolay" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


