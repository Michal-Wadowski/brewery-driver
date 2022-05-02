OBJECTS = driver.o connection.o socket.o digiport.o cJSON.o native_integration.o

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

ifndef JAVA_HOME
    JAVA_HOME = /usr/lib/jvm/java-11-openjdk-amd64
endif

LDFLAGS	= -L/usr/local/lib
LDLIBS = -lpthread -lm -lrt -lcrypt
INCLUDES = -I"$(JAVA_HOME)/include" -I"$(JAVA_HOME)/include/linux"

all:
	@echo "[Build all]"
	make jni


%.o: %.cpp
	$(CXX) -fPIC $(ARGS) $(CFLAGS) $(INCLUDES) -c $? -o $@

jni: $(OBJECTS)
	$(CXX) -shared -fPIC -o brewery_driver.so *.o -lc

clean:
	rm -f *.so *.o

