################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Cosas/Alumnos.c \
../Cosas/Programa.c \
../Cosas/SUSE.c \
../Cosas/libSUSE.c 

OBJS += \
./Cosas/Alumnos.o \
./Cosas/Programa.o \
./Cosas/SUSE.o \
./Cosas/libSUSE.o 

C_DEPS += \
./Cosas/Alumnos.d \
./Cosas/Programa.d \
./Cosas/SUSE.d \
./Cosas/libSUSE.d 


# Each subdirectory must supply rules for building sources it contributes
Cosas/%.o: ../Cosas/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/Hilolay" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


