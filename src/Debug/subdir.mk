################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../LibSVMRunner.o \
../hello_gmum.o \
../libsvm_runner.o \
../svm-predict.o \
../svm-train.o \
../svm.o \
../svm_basic.o \
../svm_client.o \
../svm_flow_factory.o \
../svm_test_flow.o 

CPP_SRCS += \
../LibSVMRunner.cpp \
../svm.cpp \
../svm_basic.cpp \
../svm_client.cpp \
../svm_flow_factory.cpp \
../svm_test_flow.cpp 

C_SRCS += \
../svm-predict.c \
../svm-train.c 

OBJS += \
./LibSVMRunner.o \
./svm-predict.o \
./svm-train.o \
./svm.o \
./svm_basic.o \
./svm_client.o \
./svm_flow_factory.o \
./svm_test_flow.o 

C_DEPS += \
./svm-predict.d \
./svm-train.d 

CPP_DEPS += \
./LibSVMRunner.d \
./svm.d \
./svm_basic.d \
./svm_client.d \
./svm_flow_factory.d \
./svm_test_flow.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


