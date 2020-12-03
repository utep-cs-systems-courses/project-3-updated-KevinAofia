#Makfile configuration
CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/ 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c 
#############################################################
#added -lp2sw, -lCircle, -lShape
#added buzzer.o SM.o led.o siren-assy.0
aofia_lab3.elf: ${COMMON_OBJECTS} aofia_lab3.o wdt_handler.o buzzer.o SM.o led.o siren-assy.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd -lp2sw -lCircle -lShape

final: aofia_lab3.elf
	msp430loader.sh $^
#############################################################
clean:
	rm -f *.o *.elf
#############################################################
#buzzer dependencies
#buzzer.o: buzzer.h

#led dependencies
#led.o: led.h

#SM dependencies
#SM.o: led.h SM.h buzzer.h
