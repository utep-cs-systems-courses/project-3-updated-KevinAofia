#Makfile configuration
CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/ 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all:aofia.elf
#############################################################
aofia.elf: ${COMMON_OBJECTS} aofia.o wdt_handler.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd 

load: aofia.elf
	msp430loader.sh $^
#############################################################
aofia1.elf: ${COMMON_OBJECTS} aofia1.o wdt_handler.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd 

load1: aofia1.elf
	msp430loader.sh $^
#############################################################
#added -lp2sw,
aofia_lab3.elf: ${COMMON_OBJECTS} aofia_lab3.o wdt_handler.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd -lp2sw

load_lab3: aofia_lab3.elf
	msp430loader.sh $^
#############################################################
clean:
	rm -f *.o *.elf
