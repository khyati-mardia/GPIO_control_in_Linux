CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		RGBLed_1.o Gpio_func.o InputSignal.o GPIOPinInfo.o

LIBS =     -pthread

CXX = i586-poky-linux-g++

PATH := $(PATH):/opt/iot-devkit/1.7.2/sysroots/x86_64-pokysdk-linux/usr/bin/i586-poky-linux

TARGET =	RGBLed_1

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
