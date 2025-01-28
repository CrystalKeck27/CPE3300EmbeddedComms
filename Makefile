PROJECT_NAME := $(shell basename $(CURDIR))
BUILD_RESOURCES_DIR := Debug/resources
OBJ_DIR := Debug/Obj

CC := arm-none-eabi-gcc
DEPS := $(wildcard Src/*.h)
SRC := $(wildcard Src/*.c)
_OBJ := $(SRC:.c=.o)
OBJ := $(patsubst Src/%, $(OBJ_DIR)/%, $(_OBJ))
OBJ_S := $(OBJ_DIR)/startup_stm32f411retx.o

BUILD_ARTIFACT_NAME := Debug/$(PROJECT_NAME)
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT := $(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

FLASH_LD := $(BUILD_RESOURCES_DIR)/STM32F411RETX_FLASH.ld
RAM_LD := $(BUILD_RESOURCES_DIR)/STM32F411RETX_RAM.ld
STARTUP_SRC := $(BUILD_RESOURCES_DIR)/startup_stm32f411retx.s

all: $(BUILD_ARTIFACT)

obj_dir:
	mkdir -p Debug/Obj

$(OBJ_S): $(STARTUP_SRC) obj_dir
	$(CC) -mcpu=cortex-m4 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(OBJ_DIR)/startup_stm32f411retx.d" -MT"$(OBJ_DIR)/startup_stm32f411retx.o" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$(OBJ_DIR)/startup_stm32f411retx.o" "$(STARTUP_SRC)"

Debug/Obj/%.o: Src/%.c $(DEPS) obj_dir
	$(CC) "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DNUCLEO_F411RE -DSTM32 -DSTM32F4 -DSTM32F411RETx -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

$(BUILD_ARTIFACT) $(BUILD_ARTIFACT_NAME).map: $(OBJ) $(OBJ_S)
	arm-none-eabi-gcc -o "$(BUILD_ARTIFACT)" $^ -mcpu=cortex-m4 -T"$(FLASH_LD)" -g3 --specs=nosys.specs -Wl,-Map="$(BUILD_ARTIFACT_NAME).map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group

clean:
	rm -f $(BUILD_ARTIFACT) $(BUILD_ARTIFACT_NAME).map $(OBJ) $(OBJ_DIR)/*.d $(OBJ_DIR)/*.o $(OBJ_DIR)/*.su

.PHONY: all build clean obj_dir