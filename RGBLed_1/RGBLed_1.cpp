#define _POSIX_C_SOURCE 200809L
#define __USE_GNU
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <iostream>
#include <list>
#include <sstream>
#include <pthread.h>
#include <linux/input.h>
#include <map>
#include <bits/stdc++.h>
#include <signal.h>
#include <string>
#include <errno.h>
#include <ctype.h>
#include "GPIOPinInfo.h"
#include "InputSignal.h"
#include "Gpio_func.h"
using namespace std;
#define MOUSEFILE "/dev/input/event2"
int readFileInputs(string);
void* notifyWhenMouseClickOccurs(void* params);
map<int, GPIOPinInfo> gpioMap;
bool flag = 1;
std::list<InputSignal> inputSignalslist;

void initialisePinsDataStructure();

int setup(int& IO1, int userdefinedValue) {

	int gpioPinNUmber;
	cout << " The input IO pin is " << IO1 << "\n";
	//Looking up map to get the gpio number from the GPIO map.
	std::map<int, GPIOPinInfo>::iterator itNew = gpioMap.find(IO1);
	if (itNew != gpioMap.end()) {
		gpioMap.erase(itNew);
	}
	cout << "Setting linux pin" << itNew->second.getLinuxPin() << "\n";
	//Linux Pin
	gpio_export(itNew->second.getLinuxPin());
	gpio_set_dir(itNew->second.getLinuxPin(), 1);
	gpioPinNUmber = itNew->second.getLinuxPin();
	gpio_set_value(itNew->second.getLinuxPin(), userdefinedValue);

	cout << "Direction pin of is" << itNew-> second.getDirectionPin() << "\n";
	//Direction Pin
	if ((itNew->second.getDirectionPin() != 81)) {
		cout << "Setting Direction PIn \n" << itNew->second.getDirectionPin() << "\n";
		gpio_export(itNew->second.getDirectionPin());
		gpio_set_dir(itNew->second.getDirectionPin(), 1);
		gpio_set_value(itNew->second.getDirectionPin(), 0);
	}
	//Mux 1 pin.
	if ((itNew->second.getMuxPin1() != 81)) {
		cout << "Setting Mux PIn 1 \n" << itNew->second.getMuxPin1() << "\n";
		gpio_export(itNew->second.getMuxPin1());
		gpio_set_dir(itNew->second.getMuxPin1(), 1);
		gpio_set_value(itNew->second.getMuxPin1(), 0);
	}
	//Mux 2 pin.
	if ((itNew->second.getMuxPin2() != 81)) {
		cout << "Setting Mux PIn 2 \n" << itNew->second.getMuxPin2() << "\n";
		gpio_export(itNew->second.getMuxPin2());
		gpio_set_dir(itNew->second.getMuxPin2(), 1);
		gpio_set_value(itNew->second.getMuxPin2(), 0);
	}
	return gpioPinNUmber;
}

void glowLedsInSequence() {
	//Taking the first line of file
	std::list<InputSignal>::iterator it = inputSignalslist.begin();
	// Duty cycle.
	int duration = it->durationOfOnPeriod*20/100000;
	duration = 1;
	int remainingDuration = 0.02 - duration;
	remainingDuration = 1;
	//IO Pins.
	int IO1 = it->redLedOn;
	int IO2 = it->greenLed;
	int IO3 = it->blueLed;
	//GPIO Pins.
	int gpioForIO1 = setup(IO1,0);
	int gpioForIO2 = setup(IO2,0);
	int gpioForIO3 = setup(IO3,0);
	int cycles =25;
	cycles =5;

	while(1) {
		//RED LED ON
		label1:            for (int i=0;i<cycles;i++) {
			gpio_set_value(gpioForIO1, 1);
			sleep(duration);
			gpio_set_value(gpioForIO1, 0);
			sleep(remainingDuration);
			flag=1;
		}
		// GREEN LED ON
		for (int i= 0;i<cycles;i++) {
			if (flag==0) {
				goto label1;
			}
			gpio_set_value(gpioForIO2,1);
			sleep(duration);
			gpio_set_value(gpioForIO2, 0);
			sleep(remainingDuration);
		}
		// BLUE LED ON
		for (int i=0;i<cycles;i++) {
			if (flag==0) {
				goto label1;
			}
			gpio_set_value(gpioForIO3,1);
			sleep(duration);
			gpio_set_value(gpioForIO3,0);
			sleep(remainingDuration);
		}
		// RED AND GREEN LED ON
		for (int i=0;i<cycles;i++) {
			if (flag==0) {
				goto label1;
			}
			gpio_set_value(gpioForIO1,1);
			gpio_set_value(gpioForIO2,1);
			sleep(duration);
			gpio_set_value(gpioForIO1,0);
			gpio_set_value(gpioForIO2,0);
			sleep(remainingDuration);
		}
		//RED AND BLUE LED ON
		for (int i=0;i<cycles && flag;i++) {
			if (flag==0) {
				goto label1;
			}
			gpio_set_value(gpioForIO1,1);
			gpio_set_value(gpioForIO3,1);
			sleep(duration);
			gpio_set_value(gpioForIO1,0);
			gpio_set_value(gpioForIO3,0);
			sleep(remainingDuration);
		}
		//GREEN AND BLUE LED ON.
		for (int i=0;i<cycles && flag;i++) {
			if (flag==0) {
				goto label1;
			}
			gpio_set_value(gpioForIO2,1);
			gpio_set_value(gpioForIO3,1);
			sleep(duration);
			gpio_set_value(gpioForIO2,0);
			gpio_set_value(gpioForIO3,0);
			sleep(remainingDuration);
		}
		//RED , GREEN AND BLUE LED ON
		for (int i=0;i<cycles && flag;i++) {
			if (flag==0) {
				goto label1;
			}
			gpio_set_value(gpioForIO1,1);
			gpio_set_value(gpioForIO2,1);
			gpio_set_value(gpioForIO3,1);
			sleep(duration);
			gpio_set_value(gpioForIO1,0);
			gpio_set_value(gpioForIO2,0);
			gpio_set_value(gpioForIO3,0);
			sleep(remainingDuration);
		}
	}
}


int main()
{
	//Reading file path to read the input file.
	string path;
	cout << "Please enter the file path,give a valid complete path inclusive of file name \n";
	cin >> path;
	//Reading the file and populating the pins inputted by the user
	readFileInputs(path);
	//Initialising the pwm pin information mapping.
	initialisePinsDataStructure();
	//Creating a reading thread that would detect the mouse events
	pthread_t readingThread;
	int s = pthread_create(&readingThread, NULL, notifyWhenMouseClickOccurs,NULL);
	if (s != 0) {
		perror("pthread_create() error");
	}
	glowLedsInSequence();
}
/**
 * THis method notifies about the mouse events.
 */
void* notifyWhenMouseClickOccurs(void* dummy) {
	int fd;
	input_event mouseEvent;
	if((fd = open(MOUSEFILE, O_RDONLY)) == -1) {
		perror("opening device");
		exit(EXIT_FAILURE);
	}
	cout << " Mouse File Opened " << "\n";
	while (1) {
		while (read(fd, &mouseEvent, sizeof(struct input_event))) {
			if ((mouseEvent.type == 1 && mouseEvent.code == 272
					&& mouseEvent.value == 1) || (mouseEvent.type == 1 && mouseEvent.code == 273
							&& mouseEvent.value == 1)) {
				flag =0;
				cout <<" Mouse_Clicked \n";
			};
		}
	}
	return 0;
}
/**
 * Initialising the data mapping of pins.
 */
void initialisePinsDataStructure() {
	//Digital GPIO Pin Mappings.
	gpioMap[0] = GPIOPinInfo(11, 32, 81, 81);
	gpioMap[1] = GPIOPinInfo(12, 28, 45 ,81);
	gpioMap[2] = GPIOPinInfo(13, 34, 77 ,81);
	gpioMap[3] = GPIOPinInfo(14, 16, 76, 64);
	gpioMap[4] = GPIOPinInfo(6, 36, 81, 81);
	gpioMap[5] = GPIOPinInfo(0, 18, 66, 81);
	gpioMap[6] = GPIOPinInfo(1, 20, 68, 81);
	gpioMap[7] = GPIOPinInfo(38, 81, 81, 81);
	gpioMap[8] = GPIOPinInfo(40, 81, 81, 81);
	gpioMap[9] = GPIOPinInfo(4, 22, 70, 81);
	gpioMap[10] = GPIOPinInfo(10, 26, 74, 81);
	gpioMap[11] = GPIOPinInfo(5, 24, 44, 72);
	gpioMap[12] = GPIOPinInfo(15, 42, 81, 81);
	gpioMap[13] = GPIOPinInfo(7, 30, 46, 81);
	//Analog Pin Mappings.
	gpioMap[14] = GPIOPinInfo(48, 81, 81, 81);
	gpioMap[15] = GPIOPinInfo(50, 81, 81, 81);
	gpioMap[16] = GPIOPinInfo(52, 81, 81, 81);
	gpioMap[17] = GPIOPinInfo(54, 81, 81, 81);
	gpioMap[18] = GPIOPinInfo(56, 81, 60, 78);
	gpioMap[19] = GPIOPinInfo(58,81,60,79);
}

/**
 * THis method is used for reading the file inputs
 */

int readFileInputs(string path) {
	//Iitialising the variables
	ifstream inFile;
	inFile.open(path.c_str());
	std::string line;
	int onDuration;
	int redPin;
	int greenPin;
	int bluePin;
	string str;
	while (inFile.good()) {
		if (!inFile.eof()) {
			if (std::getline(inFile, line)) {
				//Gets the file line by line
				std::istringstream myStreamThing(line);
				myStreamThing >> onDuration;
				InputSignal inputSignal;
				inputSignal.setDurationOfOnPeriod(onDuration);
				myStreamThing >> redPin;
				int variable = redPin;
				if (variable >= 0 && variable <= 13) {
					cout << redPin << " is the Red pin \n" ;
					inputSignal.setRedLedOn(redPin);
				} else {
					cout << "This is not a GPIO pin \n";
					exit(-1);
				}
				myStreamThing >> greenPin;
				variable = greenPin;
				if (variable >= 0 && variable <= 13) {
					inputSignal.setGreenLed(greenPin);
					cout << greenPin << " is the green Pin \n";
				} else {
					cout << "This is not a GPIO pin \n";
					exit(-1);
				}
				myStreamThing >> bluePin;
				variable = bluePin;
				if (variable >= 0 && variable <= 13) {
					cout << bluePin << " is the Blue Pin \n";
					inputSignal.setBlueLed(bluePin);
				} else {
					cout << "This is not a GPIO Pin \n";
					exit(-1);
				}
				inputSignalslist.push_back(inputSignal);
				myStreamThing.clear();
				line.clear();
			}

		}

	}

	inFile.close();
	return (0);
}
