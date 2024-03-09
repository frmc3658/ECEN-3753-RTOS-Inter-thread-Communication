################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Middlewares/Third_Party/SystemView/Sample/FreeRTOS/Config/subdir.mk
-include Middlewares/Third_Party/SystemView/Sample/FreeRTOS/subdir.mk
-include Middlewares/Third_Party/SystemView/SEGGER/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := McDermott_Frank_RTOS_Lab6_ITC
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
McDermott_Frank_RTOS_Lab6_ITC.elf \

MAP_FILES += \
McDermott_Frank_RTOS_Lab6_ITC.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
McDermott_Frank_RTOS_Lab6_ITC.list \


# All Target
all: main-build

# Main-build Target
main-build: McDermott_Frank_RTOS_Lab6_ITC.elf secondary-outputs

# Tool invocations
McDermott_Frank_RTOS_Lab6_ITC.elf McDermott_Frank_RTOS_Lab6_ITC.map: $(OBJS) $(USER_OBJS) C:\Users\Public\Desktop\Code\Working\C\ECEN\ 3753\Lab6\lab-6-itc-frmc3658\McDermott_Frank_RTOS_Lab6_ITC\STM32F429ZITX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "McDermott_Frank_RTOS_Lab6_ITC.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Public\Desktop\Code\Working\C\ECEN 3753\Lab6\lab-6-itc-frmc3658\McDermott_Frank_RTOS_Lab6_ITC\STM32F429ZITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="McDermott_Frank_RTOS_Lab6_ITC.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

McDermott_Frank_RTOS_Lab6_ITC.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "McDermott_Frank_RTOS_Lab6_ITC.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) McDermott_Frank_RTOS_Lab6_ITC.elf McDermott_Frank_RTOS_Lab6_ITC.list McDermott_Frank_RTOS_Lab6_ITC.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
