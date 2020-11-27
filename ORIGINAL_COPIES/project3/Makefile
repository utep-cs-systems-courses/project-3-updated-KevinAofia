# makfile configuration
COMMON_OBJECTS  = 
CPU             = msp430g2553
CFLAGS          = -mmcu=${CPU} -I../h
LDFLAGS		= -L/opt/ti/msp430_gcc/include

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-as

all: buzzyblink.elf 

#rules for files:
#we need to make our target dependent on the files we merged over(buzzer and button)
buzzyblink.elf: ${COMMON_OBJECTS} main.o SM.o led.o wdIH.o buzzer.o wdIH.o switches.o p2IH.o ../lib/libTimer.a
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^

load: buzzyblink.elf
	msp430loader.sh buzzyblink.elf

clean:
	rm -f *.o *.elf

#led dependencies
led.o: led.c led.h

#wdIH dependencies (added buzzer.h led.h switches.h)
wdIH.o: SM.h buzzer.h led.h switches.h

#SM dependencies (added buzzer.h)
SM.o: led.h SM.h buzzer.h

#added all dependecies below this line

#main dependencies
main.o: led.h buzzer.h switches.h

#buzzer dependencies
buzzer.o: buzzer.h

#switches dependencies
switches.o: switches.h led.h


