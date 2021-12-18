#INCLUDE	= -I/usr/local/include
# CFLAGS	= -Wall $(INCLUDE) -Winline -pipe

ifeq ($(REAL_IO),1)
	override REAL_IO := TM1637Display.cpp -lwiringPi -lwiringPiDev -DREAL_IO
	CXX	= arm-linux-gnueabihf-g++
endif

ifeq ($(BLUETOOTH),1)
	override BLUETOOTH = -DBLUETOOTH
endif

ifeq ($(DEBUG),1)
	override DEBUG = -DDEBUG
endif

override ARGS = $(REAL_IO) $(BLUETOOTH) $(DEBUG)



LDFLAGS	= -L/usr/local/lib
# LDLIBS    = TM1637Display.cpp -lwiringPi -lwiringPiDev -lpthread -lm -lrt -lcrypt
LDLIBS    = -lpthread -lm -lrt -lcrypt

# all:
# 	@echo "[Build all]"
# 	make driver
# #	python3 build_checksum.py


%.o: %.cpp
	$(CXX) $(ARGS) $(CFLAGS) $(INCLUDES) -c $? -o $@

OBJECTS = demo.o connection.o socket.o digiport.o cJSON.o

demo: $(OBJECTS)
	$(CXX) *.o $(ARGS) $(CFLAGS) $(LDLIBS) $(INCLUDES) -o $@

clean:
	rm -f driver driver-* demo *.o

#install:
#	sudo cp driver-* /usr/local/bin/driver

