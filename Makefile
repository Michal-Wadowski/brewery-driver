#INCLUDE	= -I/usr/local/include
# CFLAGS	= -Wall $(INCLUDE) -Winline -pipe

OBJECTS = driver.o connection.o socket.o digiport.o cJSON.o

ifeq ($(REAL_IO),1)
	CXX	= arm-linux-gnueabihf-g++
	override REAL_IO := -lwiringPi -lwiringPiDev -DREAL_IO
	override OBJECTS := $(OBJECTS) TM1637Display.o
endif

ifeq ($(BLUETOOTH),1)
	override BLUETOOTH = -DBLUETOOTH
endif

ifeq ($(DEBUG),1)
	override DEBUG = -DDEBUG
endif

override ARGS = $(REAL_IO) $(BLUETOOTH) $(DEBUG)



LDFLAGS	= -L/usr/local/lib
LDLIBS    = -lpthread -lm -lrt -lcrypt

all:
	@echo "[Build all]"
	make driver
	python3 build_checksum.py


%.o: %.cpp
	$(CXX) $(ARGS) $(CFLAGS) $(INCLUDES) -c $? -o $@

driver: $(OBJECTS)
	$(CXX) *.o $(ARGS) $(CFLAGS) $(LDLIBS) $(INCLUDES) -o $@

clean:
	rm -f driver driver-* driver *.o

install:
	sudo cp driver-* /usr/local/bin/driver

