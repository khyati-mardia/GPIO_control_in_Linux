/*
 * PWMPinInfo.h
 * This PWMPinInfo class holds all the information of the PWM Pin.
 * This class holds the linux pwmPin number , direction pin number and other mux pins
 * related to that IO pin.
 *  Created on: Sep 29, 2018
 *      Author: Chandrika.
 */

#ifndef PWMPinInfo_H_
#define PWMPinInfo_H_

class PWMPinInfo {
	int pwmPin;
	int directionPin;
	int muxPin1;
	int muxPin2;
public:
	PWMPinInfo();
	virtual ~PWMPinInfo();

	PWMPinInfo(int linuxPinParam) { // @suppress("Class members should be properly initialized")
		pwmPin = linuxPinParam;
	}

	PWMPinInfo(int linuxPinParam, int directionPinParam) { // @suppress("Class members should be properly initialized")
		pwmPin = linuxPinParam;
		directionPin = directionPinParam;
	}

	PWMPinInfo(int linuxPinParam, int directionPinParam, int muxPinParam1) { // @suppress("Class members should be properly initialized")
		pwmPin = linuxPinParam;
		directionPin = directionPinParam;
		muxPin1 = muxPinParam1;
	}

	PWMPinInfo(int linuxPinParam, int directionPinParam, int muxPinParam1, int muxPinParam2) {
		pwmPin = linuxPinParam;
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

	int getPWMPin() const {
		return pwmPin;
	}

	void setPWMPin(int pwmPIn) {
		this->pwmPin = pwmPIn;
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

#endif /* PWMPinInfo_H */
