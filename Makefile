ifneq ($V,1)
Q ?= @
endif


#DEBUG	= -O3
CXX	= arm-linux-gnueabihf-g++
INCLUDE	= -I/usr/local/include
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS	= -L/usr/local/lib
LDLIBS    = -lwiringPi -lwiringPiDev -lpthread -lm -lrt


OBJ	=	$(SRC:.cpp=.o)

BINS	=	$(SRC:.cpp=)

all:
	$Q echo "[Build]"
	$Q make driver
	python3 build_checksum.py

clean:
	$Q echo "[Clean]"
	$Q rm -f driver driver-*

install:
	$Q sudo cp driver-* /usr/local/bin/driver

