/*
 * GPIOPinInfo.h
 * This class is used to hold values of the GPIO pin mapping.
 * In order to turn on the GPIO pin , we need to make sure the corresponding
 * pins are also turned on.
 * This mapping information is stored in the form of a class .
 *  Created on: Sep 29, 2018
 *      Author: chandrika
 */

#ifndef GPIOPININFO_H_
#define GPIOPININFO_H_

class GPIOPinInfo {
	int linuxPin;
	int directionPin;
	int muxPin1;
	int muxPin2;
public:
	GPIOPinInfo();
	virtual ~GPIOPinInfo();

	GPIOPinInfo(int linuxPinParam) { // @suppress("Class members should be properly initialized")
		linuxPin = linuxPinParam;
	}

	GPIOPinInfo(int linuxPinParam, int directionPinParam) { // @suppress("Class members should be properly initialized")
		linuxPin = linuxPinParam;
		directionPin = directionPinParam;
	}

	GPIOPinInfo(int linuxPinParam, int directionPinParam, int muxPinParam1) { // @suppress("Class members should be properly initialized")
		linuxPin = linuxPinParam;
		directionPin = directionPinParam;
		muxPin1 = muxPinParam1;
	}

	GPIOPinInfo(int linuxPinParam, int directionPinParam, int muxPinParam1, int muxPinParam2) {
		linuxPin = linuxPinParam;
		directionPin = directionPinParam;
		muxPin1 = muxPinParam1;
		muxPin2 = muxPinParam2;
	}

	int getDirectionPin() const {
		return directionPin;
	}

	void setDirectionPin(int directionPin) {
		this->directionPin = directionPin;
	}

	int getLinuxPin() const {
		return linuxPin;
	}

	void setLinuxPin(int linuxPin) {
		this->linuxPin = linuxPin;
	}

	int getMuxPin1() const {
		return muxPin1;
	}

	void setMuxPin1(int muxPin) {
		this->muxPin1 = muxPin;
	}

	int getMuxPin2() const {
		return muxPin2;
	}

	void setMuxPin2(int muxPin2) {
		this->muxPin2 = muxPin2;
	}
};

#endif /* GPIOPININFO_H_ */
