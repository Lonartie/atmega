ROBO_BINARY 	= robo
ROBO_HEX 			= robo.hex

CORE_DIR			= robot/core

OBJ_DIR				= obj
ROBO_OBJ_DIR 	= $(OBJ_DIR)/robo
ROBOT_SRC_DIR	= robot/robot

DOXY_OUT_DIR  = doxygen

ROBO_GCC 			= avr-gcc
ROBO_OBJ_COPY = avr-objcopy
ROBO_DUDE 		= avrdude

ROBO_FLAGS 		= -ffunction-sections -fdata-sections
ROBO_FLAGS 	 	+= -Wl,--gc-sections -Os -flto
ROBO_FLAGS 	 	+= -Wall -Wextra -Wfatal-errors -Wno-unused-variable
ROBO_FLAGS 	 	+= -D F_CPU=16000000 -mmcu=atmega328p
ROBO_FLAGS 	 	+= -D MAIN=main -I $(CORE_DIR)

ROBO_SOURCES 	:= $(shell find $(ROBOT_SRC_DIR) -name '*.c') $(shell find $(CORE_DIR) -name '*.c') 
ROBO_OBJECTS 	:= $(addprefix $(ROBO_OBJ_DIR)/, $(subst .c,.o,$(ROBO_SOURCES)))
ROBO_DEPENDS 	:= $(addprefix $(ROBO_OBJ_DIR)/, $(subst .c,.d,$(ROBO_SOURCES)))

# execute even if exists
.PHONY: all clean $(ROBO_BINARY)_flash Doxygen

# default target, build all
all: Doxygen $(ROBO_BINARY)_flash

# clean build files
clean:
	@echo "Deleting $(OBJ_DIR)"
	@echo "Deleting $(ROBO_HEX)"
	@echo "Deleting $(ROBO_BINARY)"
	@echo "Deleting Doxygen Output"
	@$(RM) -rf $(OBJ_DIR) $(TEST_OBJ_DIR) $(DOXY_OUT_DIR)
	@$(RM) -f $(ROBO_BINARY) $(ROBO_HEX) $(TEST_BINARY)

# compile binary for robot
$(ROBO_BINARY): $(ROBO_OBJECTS)
	@echo "Linking $(ROBO_BINARY)"
	@$(ROBO_GCC) $(ROBO_FLAGS) $^ -o $(ROBO_BINARY)

# extracting hex for robot
$(ROBO_HEX): $(ROBO_BINARY)
	@echo "Creating $(ROBO_HEX)"
	@$(ROBO_OBJ_COPY) -O ihex $(ROBO_BINARY) $(ROBO_HEX)

# flashing to robot
$(ROBO_BINARY)_flash: $(ROBO_HEX)
	@echo "Flashing $(ROBO_BINARY)"
	@$(ROBO_DUDE) -p atmega328p -c arduino -P /dev/ttyACM0 -b 115200 -U flash:w:$(ROBO_HEX):i

# Doxygen
Doxygen:
	@echo "Generating Doxygen"
	@doxygen
	@echo "Doxygen generated"

# recompile sources if dependent header files changed
-include $(ROBO_DEPENDS)

# recompile sources if source files changed
$(ROBO_OBJ_DIR)/$(ROBOT_SRC_DIR)/%.o: $(ROBOT_SRC_DIR)/%.c Makefile
	@mkdir -p $(dir $@)
	@echo "Compiling $(basename $<)"
	@$(ROBO_GCC) $(ROBO_FLAGS) -MMD -MP -c $< -o $@

# recompile sources if source files changed
$(ROBO_OBJ_DIR)/$(CORE_DIR)/%.o: $(CORE_DIR)/%.c Makefile
	@mkdir -p $(dir $@)
	@echo "Compiling $(basename $<)"
	@$(ROBO_GCC) $(ROBO_FLAGS) -MMD -MP -c $< -o $@
