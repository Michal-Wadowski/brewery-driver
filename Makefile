OBJECTS = digiport.o DigiPort.o Application.o debug.o

LDLIBS = -lpthread -lm -lrt -lcrypt

ifeq ($(REAL_IO),1)
	CXX	= arm-linux-gnueabihf-g++
	override LDLIBS := $(LDLIBS) -lwiringPi -lwiringPiDev
	override REAL_IO := -DREAL_IO
	override OBJECTS := $(OBJECTS) TM1637Display.o
endif

override ARGS = $(REAL_IO)

ifndef JAVA_HOME
    JAVA_HOME = /usr/lib/jvm/java-11-openjdk-amd64
endif

LDFLAGS	= -L/usr/local/lib
INCLUDES = -I"$(JAVA_HOME)/include" -I"$(JAVA_HOME)/include/linux"

all:
	@echo "[Build Orange Pi driver]"
	make jni

demo:
	@echo "[Build demo driver]"
	make jni-demo

%.o: %.cpp
	$(CXX) -fPIC $(ARGS) $(CFLAGS) $(INCLUDES) -c $? -o $@

jni: $(OBJECTS)
	$(CXX) -shared -fPIC -o brewery_driver.so *.o -lc

jni-demo: $(OBJECTS)
	$(CXX) -shared -fPIC -o brewery_driver_demo.so *.o -lc

clean:
	rm -f *.so *.o
