CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		RGBLed_2.o PWMPinInfo.o InputSignal.o Gpio_func.o 

LIBS = -pthread

CXX = i586-poky-linux-g++

PATH := $(PATH):/opt/iot-devkit/1.7.2/sysroots/x86_64-pokysdk-linux/usr/bin/i586-poky-linux

TARGET =	RGBLed_2

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
