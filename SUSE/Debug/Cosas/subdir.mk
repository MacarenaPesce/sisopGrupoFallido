################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Cosas/SUSE.c 

OBJS += \
./Cosas/SUSE.o 

C_DEPS += \
./Cosas/SUSE.d 


# Each subdirectory must supply rules for building sources it contributes
Cosas/%.o: ../Cosas/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/Hilolay" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


