#All variable.
PROJ_NAME = main
CC = @avr-gcc
OBJECT_COPY = @avr-objcopy
SRC = src/
OUT_DIR = out/

programmer = -c usbasp
device = -p m88p

CFLAG = -mmcu=atmega88p	\
		-I inc/							\
		-MMD								\

OBJECT = $(patsubst src/%.c,out/%.o,$(wildcard src/*.c))

.PHONY: loadflash erase loadbootloader clean hex

$(OUT_DIR)$(PROJ_NAME).hex: $(OBJECT)
	$(CC) $(CFLAG) -o $(OUT_DIR)$(PROJ_NAME).elf $(OBJECT)
	$(OBJECT_COPY) -Oihex $(OUT_DIR)$(PROJ_NAME).elf $(OUT_DIR)$(PROJ_NAME).hex

$(OUT_DIR)%.o: $(SRC)%.c
	$(CC) $(CFLAG) -c  $< -o $@

sinclude $(OUT_DIR)*.d

erase:
	@avrdude $(programmer) $(device) -e

load:	$(OUT_DIR)$(PROJ_NAME).hex
	@avrdude $(programmer) $(device) -e -U flash:w:$(OUT_DIR)$(PROJ_NAME).hex

loadbootloader: $(SCRIPT_DIR)bootloader.bin
	@JLinkExe -device stm32f103c8 -si swd -speed 4000 $(SCRIPT_DIR)loadbootloader.jlink

clean:
	@-rm -f $(OUT_DIR)*.hex $(OUT_DIR)*.o $(OUT_DIR)*.bin $(OUT_DIR)*.elf $(OUT_DIR)*.d
