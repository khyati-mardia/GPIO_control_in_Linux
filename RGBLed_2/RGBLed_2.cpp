//============================================================================
// Name        : RGBLed_2.cpp
// Author      : Chandrika C.S. , 1215321133
//               Khyati Mardia, 1215346587
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#define _POSIX_C_SOURCE 200809L
#define __USE_GNU
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <iostream>
#include <list>
#include <sstream>
#include <pthread.h>
#include <linux/input.h>
#include <map>
#include <bits/stdc++.h>
#include <pthread.h>
#include <string>
#include "PWMPinInfo.h"
#include "InputSignal.h"
#include "Gpio_func.h"

using namespace std;
#define MOUSEFILE "/dev/input/event2"
int readFileInputs(string);
void* notifyWhenMouseClickOccurs(void* params);
void setNewPWMValue(int, bool);
bool checkIfPinIsPWMOrNot(int);
void initialisePinsDataStructure();
void glowLedsInSequence();
int getPWMPin(int);
static int pwmPin1;
static int pwmPin2;
static int pwmPin3;
int flag=0;
int initialDutyCycle;
map<int, PWMPinInfo> pwmPinInfoMap;
std::list<InputSignal> inputSignalslist;

/**
 * Glows the led by giving values to all the other pins associated.
 */
void glowLed(int& IO1, int userdefinedValue) {
	cout << "The IO pin is " <<IO1 << "\n";
	//PWM Pin retrieved by looking up the map created.
	std::map<int, PWMPinInfo>::iterator itNew = pwmPinInfoMap.find(IO1);
	if (itNew != pwmPinInfoMap.end()) {
		pwmPinInfoMap.erase(itNew);
	}
	//Direction Pin
	if (itNew->second.getDirectionPin() < 80 && itNew->second.getDirectionPin() > 0) {
		cout << "The direction pin is " << itNew-> second.getDirectionPin() << "\n";
		gpio_export(itNew->second.getDirectionPin());
		gpio_set_value(itNew->second.getDirectionPin(), 0);
		gpio_set_dir(itNew->second.getDirectionPin(), O_WRONLY);
	}
	// Mux Pin 1
	if (itNew->second.getMuxPin1() < 80 && itNew->second.getMuxPin1() > 0) {
		cout << "The mux1 pin is " << itNew->second.getMuxPin1() << "\n";
		gpio_set_dir(itNew->second.getMuxPin1(), O_WRONLY);
		gpio_export(itNew->second.getMuxPin1());
		gpio_set_value(itNew->second.getMuxPin1(), 0);
	}
	// Mux pin 2.
	if (itNew->second.getMuxPin2() < 80 && itNew->second.getMuxPin2() > 0) {
		cout << "The mux2 pin is " << itNew->second.getMuxPin2() << "\n";
		gpio_export(itNew->second.getMuxPin2());
		gpio_set_dir(itNew->second.getMuxPin2(), O_WRONLY);
		gpio_set_value(itNew->second.getMuxPin2(), 0);
	}
}

int main(void) {
	//Reading file path to read the input file.
	string path;
	cout << "Please enter the file path,give a valid complete path inclusive of file name \n";
	cin >> path;
	//Reading the file and populating the values of different IO pins.
	readFileInputs(path);
	//Initialising the pin data mapping
	initialisePinsDataStructure();
	//Initialising the thread to read the mouse events
	pthread_t readingThread;
	std::list<InputSignal>::iterator it = inputSignalslist.begin();
	//PWM variables initialisation.
	int IO1 = it->redLedOn;
	int IO2 = it->greenLed;
	int IO3 = it->blueLed;
	pwmPin1 = getPWMPin(IO1);
	pwmPin2 = getPWMPin(IO2);
	pwmPin3 = getPWMPin(IO3);
	//initialising duty cycle.
	initialDutyCycle =  it->durationOfOnPeriod*0.00005;
	//Creating the reading thread.
	int s = pthread_create(&readingThread, NULL, notifyWhenMouseClickOccurs,NULL);
	if (s != 0) {
		perror("pthread_create() error");
	}
	//Led glowing sequence.
	glowLedsInSequence();
}
/**
 * Exports the PWM pin.
 */
int pwm_export(unsigned int pwmPinNumber) {
	int fd, len;
	char buf[MAX_BUF];
	fd = open("/sys/class/pwm/pwmchip0/export", O_WRONLY);
	if (fd < 0) {
		perror("pwm/export");
		return fd;
	}

	len = snprintf(buf, sizeof(buf), "%d", pwmPinNumber);
	write(fd, buf, len);
	cout << "PWM Pin "<<  pwmPinNumber << "exported \n";
	close(fd);
	return 0;
}
/**
 * Enables the PWM pin.
 */
int pwm_enable(int pwmPin, unsigned int pwm_enable_pin) {
	int fd;
	char buf[MAX_BUF];
	snprintf(buf, sizeof(buf), "/sys/class/pwm/pwmchip0/pwm%d/enable", pwmPin);
	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("pwm/enable");
		return fd;
	}
	if(pwm_enable_pin){
		cout << " PWM Pin " << pwmPin << " Enabled" << "\n";
		write(fd,"1",1);
	}
	else
		write(fd,"0",1);
	close(fd);
	return 0;
}
/**
 * Sets the duty cycle for a particular PWM pin.
 */
int pwm_duty_cycle(int pwmPin,unsigned long int pwm_duty) {
	int fd;
	char buf[MAX_BUF], buf1[MAX_BUF];
	snprintf(buf, sizeof(buf),"/sys/class/pwm/pwmchip0/pwm%d/duty_cycle",pwmPin);
	fd = open(buf, O_WRONLY);
	if (fd <0) {
		cout << pwmPin << "\n";
		perror("pwm/dutycycle");
		return fd;
	}
	snprintf(buf1, sizeof(buf1), "%lu", pwm_duty);
	if (pwm_duty) {
		printf("setting the new duty cycle %lu for PWMPin Number of %d \n" ,pwm_duty, pwmPin);
		write(fd,buf1, sizeof(buf1));
	}
	else
		write(fd,"0",1);
	close(fd);
	return 0;

}

/**
 * THis method notifies the user whenever there is a left click or a right click.
 */
void* notifyWhenMouseClickOccurs(void* dummy) {
	int fd;
	input_event mouseEvent;
	if((fd = open(MOUSEFILE, O_RDONLY)) == -1) {
		perror("opening device");
		exit(EXIT_FAILURE);
	}
	cout << "Mouse file is opened \n";
	while (1) {
		while (read(fd, &mouseEvent, sizeof(struct input_event))) {
			if ((mouseEvent.type == 1 && mouseEvent.code == 272
					&& mouseEvent.value == 1)) {
				cout << "Left mouse clicked \n";
				setNewPWMValue(pwmPin1, true);
				setNewPWMValue(pwmPin2, true);
				setNewPWMValue(pwmPin3, true);
			}
			else if ((mouseEvent.type == 1 && mouseEvent.code == 273
					&& mouseEvent.value == 1)) {
				cout << "Right Mouse clicked \n";
				setNewPWMValue(pwmPin1, false);
				setNewPWMValue(pwmPin2, false);
				setNewPWMValue(pwmPin3, false);
			}
		}
	}
	return 0;
}
/**
 *  Setting the PWM value based on boolean value of increment/decrement.
 */
void setNewPWMValue(int pwmPin, bool incrementOrDecrement ) {
	if(incrementOrDecrement) {
		initialDutyCycle = 1.1*initialDutyCycle;
		cout<<"Incremented value is " << initialDutyCycle << "\n";
	}
	else {
		initialDutyCycle = 0.9*initialDutyCycle;
		cout<<"Decremented value"<<initialDutyCycle << "\n";

	}
	pwm_duty_cycle(pwmPin, initialDutyCycle);
}

/**
 * Setting up the PWM pins.
 */
int PWMSetup(int& IO1, int userdefinedValue) {
	int pwmPinNumber;
	cout << "THE IO PIN GOING ON IS " << IO1 << "\n";
	std::map<int, PWMPinInfo>::iterator itNew = pwmPinInfoMap.find(IO1);
	if (itNew == pwmPinInfoMap.end()) {
		pwmPinInfoMap.erase(itNew);
		cout << "Matching Pin Information not found or No pin input received";
		exit(-1);
	}
	else {
		//Setting up PWM pin.
		cout << "Setting PWM pin " << itNew->second.getPWMPin() << "\n";
		pwmPinNumber = itNew->second.getPWMPin();
		pwm_export(pwmPinNumber);
		pwm_enable(pwmPinNumber,1);
		initialDutyCycle = userdefinedValue*50000;
		cout<<"Initial Duty Cycle given by me after calc is" << initialDutyCycle;
		pwm_duty_cycle(pwmPinNumber,initialDutyCycle);


		if ((itNew->second.getDirectionPin() != 81)) {
			cout << "Setting Direction PIn " << itNew->second.getDirectionPin() << "\n";
			gpio_export(itNew->second.getDirectionPin());
			gpio_set_dir(itNew->second.getDirectionPin(), 1);
			gpio_set_value(itNew->second.getDirectionPin(), 0);
		}
		if ((itNew->second.getMuxPin1() != 81)) {
			cout << "Setting Mux PIn 1 " << itNew->second.getMuxPin1() << "\n";
			gpio_export(itNew->second.getMuxPin1());
			gpio_set_dir(itNew->second.getMuxPin1(), 1);
			int muxPin = itNew->second.getMuxPin1();
			if (muxPin == 76 || muxPin == 70) {
				gpio_set_value(muxPin, 0);
			}
			else if (muxPin == 66 || muxPin == 68 || muxPin == 74) {
				gpio_set_value(muxPin, 1);
			}
		}
		if ((itNew->second.getMuxPin2() != 81)) {
			cout << "Setting Mux PIn 2 " << itNew->second.getMuxPin2() << "\n";
			gpio_export(itNew->second.getMuxPin2());
			gpio_set_dir(itNew->second.getMuxPin2(), 1);
			gpio_set_value(itNew->second.getMuxPin2(), 1);
		}
	}
	return pwmPinNumber;
}

int getPWMPin(int IO1) {
	int pwmPin = 0;
	std::map<int, PWMPinInfo>::iterator itNew = pwmPinInfoMap.find(IO1);
	if (itNew != pwmPinInfoMap.end()) {
		pwmPin = itNew->second.getPWMPin();
	}
	return pwmPin;
}

void glowLedsInSequence() {
	//Reading the first line input of the file.
	std::list<InputSignal>::iterator it = inputSignalslist.begin();
	int dutyCycle = it->durationOfOnPeriod;
	cout << "Duty Cycle is " << dutyCycle << "\n";
	dutyCycle = 5000000;
	int duration = dutyCycle*0.00005; //seconds
	duration = 0.5;
	int remainingDuration = (0.02 - duration);//nanoseconds
	remainingDuration = 0.5;
	int IO1 = it->redLedOn;
	int IO2 = it->greenLed;
	int IO3 = it->blueLed;
	int PWMPin1 = PWMSetup(IO1,dutyCycle);
	cout << "PWM Pin corresponding to the IO1 is" << PWMPin1 << "\n";
	int PWMPIn2 = PWMSetup(IO2,dutyCycle);
	cout << "PWM Pin corresponding to the IO2 is "<< PWMPIn2 << "\n";
	int PWMPin3 = PWMSetup(IO3,dutyCycle);
	cout << "PWM pin corresponding to the IO3 is " << PWMPin3 << "\n";

	while(1) {
		pwm_enable(PWMPin1,1);
		sleep(duration);
		pwm_enable(PWMPin1, 0);
		sleep(remainingDuration);

		pwm_enable(PWMPIn2, 1);
		sleep(duration);
		pwm_enable(PWMPIn2, 0);
		sleep(remainingDuration);

		pwm_enable(PWMPin3, 1);
		sleep(duration);
		pwm_enable(PWMPin3, 0);
		sleep(remainingDuration);

		pwm_enable(PWMPin1, 1);
		pwm_enable(PWMPIn2, 1);
		sleep(duration);
		pwm_enable(PWMPin1, 0);
		pwm_enable(PWMPIn2, 0);
		sleep(remainingDuration);

		pwm_enable(PWMPin1, 1);
		pwm_enable(PWMPin3, 1);
		sleep(duration);
		pwm_enable(PWMPin1, 0);
		pwm_enable(PWMPin3, 0);
		sleep(remainingDuration);

		pwm_enable(PWMPIn2, 1);
		pwm_enable(PWMPin3, 1);
		sleep(duration);
		pwm_enable(PWMPIn2, 0);
		pwm_enable(PWMPin3, 0);
		sleep(remainingDuration);

		pwm_enable(PWMPin1, 1);
		pwm_enable(PWMPIn2, 1);
		pwm_enable(PWMPin3, 1);
		sleep(duration);
		pwm_enable(PWMPin1, 0);
		pwm_enable(PWMPIn2, 0);
		pwm_enable(PWMPin3, 0);
		sleep(remainingDuration);
	}
}
/**
 * Initialising internal map of pins.
 */
void initialisePinsDataStructure() {
	//Setting these values for the PWM pins , if the value is valid,
	//we set the given number , else we set it to invalid number of 81.
	pwmPinInfoMap[3] = PWMPinInfo(1, 16,76,64);
	pwmPinInfoMap[5] = PWMPinInfo(3, 18, 66,81);
	pwmPinInfoMap[6] = PWMPinInfo(5, 20, 68,81);
	pwmPinInfoMap[9] = PWMPinInfo(9, 7, 22, 70);
	pwmPinInfoMap[10] = PWMPinInfo(11, 26,74, 81);
	pwmPinInfoMap[11] = PWMPinInfo(19, 24, 81,72);
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
				if (checkIfPinIsPWMOrNot(variable)) {
					cout << redPin << " is the IO1 (Red) Pin \n";
					inputSignal.setRedLedOn(redPin);
				} else {
					cout << "This pin is not enabled for PWM";
					exit(-1);
				}
				myStreamThing >> greenPin;
				variable = greenPin;
				if (checkIfPinIsPWMOrNot(variable)) {
					inputSignal.setGreenLed(greenPin);
					cout << greenPin << "is the IO2 pin Green Pin \n";
				} else {
					cout << "This pin is not enabled for PWM";
					exit(-1);
				}
				myStreamThing >> bluePin;
				variable = bluePin;
				if (checkIfPinIsPWMOrNot(variable)) {
					cout << bluePin << " is the IO3 pin i.e. Blue Pin! \n";
					inputSignal.setBlueLed(bluePin);
				} else {
					cout << "This pin is not enabled for PWM";
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

/**
 * Checks if the given pin is enabled for PWM or not
 */
bool checkIfPinIsPWMOrNot(int variable) {
	int PWMPins[] = {3,5,6,7,9, 10, 11};
	for(int i=0;i<7; i++) {
		if (PWMPins[i] == variable)
			return true;
	}
	return false;
}

