# Compiler and other Section
CC = E:\Documents\Arduino\arduino-1.8.19\hardware\tools\avr\bin\avr-gcc
OBJCOPY = E:\Documents\Arduino\arduino-1.8.19\hardware\tools\avr\bin\avr-objcopy
AVRSIZE  = E:\Documents\Arduino\arduino-1.8.19\hardware\tools\avr\bin\avr-size
#Main application file name
MAIN_APP = main

#Main hex file path in windows format

#Options for avr-gcc
CFLAGS = -g -Os -o

#Linking options for avr-gcc
LFLAGS = -Os -mmcu=attiny24 -o

#Options for HEX file generation
HFLAGS = -j .text -j .data -O ihex

# ALL_CFLAGS = -mmcu=attiny24 -I. $(CFLAGS) $(GENDEPFLAGS)

# Sources files needed for building the application 
SRC = $(MAIN_APP).c
SRC += lib.c

# The headers files needed for building the application
INCLUDE = 
INCLUDE += 

# lenh linker: avr-objdump -h -S main.elf > main.lst   
Build : $(MAIN_APP).elf
	$(OBJCOPY) $(HFLAGS) $< $(MAIN_APP).hex
	$(AVRSIZE) -C --mcu=attiny24 $(MAIN_APP).elf	

# $(MAIN_APP).bin: $(MAIN_APP).c
# 	$(CC) $(MAIN_APP).c -mmcu=attiny24 -DF_CPU=8000000UL -Os -o $(MAIN_APP).bin

$(MAIN_APP).elf: $(MAIN_APP).o
	$(CC) $(SRC) $(INCLUDE) $(LFLAGS) $@
	
$(MAIN_APP).o:$(SRC)
	$(CC) $^ $(INCLUDE) $(CFLAGS) $@

clean: 
	rm -rf $(MAIN_APP).elf $(MAIN_APP).hex $(MAIN_APP).o $(MAIN_APP).bin