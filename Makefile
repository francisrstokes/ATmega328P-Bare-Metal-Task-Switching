# Arch / MCU
MCU					?= atmega328p
MCU_UPPER		:= $(shell echo $(MCU) | tr '[:lower:]' '[:upper:]')

PROGRAMMER	?= arduino
SERIAL_PORT	?= /dev/ttyACM0
BAUD_RATE		?= 115200

# Programs
CC					:= avr-gcc
OBJCOPY			:= avr-objcopy
RM_RF				:= rm -rf
MKDIR				:= mkdir -p

# Flags
CCFLAGS			:= -Os -DF_CPU=16000000UL -mmcu=$(MCU)
LDFLAGS			:= -mmcu=$(MCU)
OCFLAGS			:= -O ihex -R .eeprom
INCLUDE			:= -I/usr/avr/include

# Files
SRC_DIRS		:= src
BUILD_DIR		:= build

SRC					:= $(shell find $(SRC_DIRS) -name "*.c" -or -name *.s)
OBJ					:= $(SRC:%=$(BUILD_DIR)/%.o)
EXE					:= main
HEX					:= main.hex

.PHONY: all
all: $(BUILD_DIR)/$(HEX)
	#

$(BUILD_DIR)/$(HEX): $(BUILD_DIR)/$(EXE)
	$(OBJCOPY) $(OCFLAGS) $< $@

$(BUILD_DIR)/$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR) $(dir $@)
	$(CC) $(INCLUDE) -c $(CCFLAGS) $< -o $@

clean:
	$(RM_RF) obj/* build/*

flash: $(BUILD_DIR)/$(HEX)
	avrdude -F -V -c $(PROGRAMMER) -p $(MCU_UPPER) -P $(SERIAL_PORT) -b $(BAUD_RATE) -U flash:w:$(BUILD_DIR)/$(HEX)
