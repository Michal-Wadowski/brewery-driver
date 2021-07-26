ifneq ($V,1)
Q ?= @
endif

#DEBUG	= -g -O0
DEBUG	= -O3
CC	= gcc
INCLUDE	= -I/usr/local/include
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS	= -L/usr/local/lib
LDLIBS    = -lwiringPi -lwiringPiDev -lpthread -lm -lcrypt -lrt


OBJ	=	$(SRC:.c=.o)

BINS	=	$(SRC:.c=)

all:
	$Q echo "[Build]"
	$Q make driver

clean:
	$Q echo "[Clean]"
	$Q rm -f driver
	
install:
	$Q sudo cp driver /usr/local/bin/driver

