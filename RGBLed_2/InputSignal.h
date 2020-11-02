/*
 * InputSignal.h
 * This class gives an information about the inputs given by the user.
 * i.e. the initial duty cycle , IO1, IO2, IO3 pins are saved in the form of
 * this class information.
 *  Created on: Sep 29, 2018
 *      Author: chandrika
 */

#ifndef INPUTSIGNAL_H_
#define INPUTSIGNAL_H_

class InputSignal {
public:
	int durationOfOnPeriod;
	int redLedOn;
	int greenLed;
	int blueLed;

public:
	InputSignal();
	virtual ~InputSignal();

	int getBlueLed() const {
		return blueLed;
	}

	void setBlueLed(int blueLed) {
		this->blueLed = blueLed;
	}

	int getDurationOfOnPeriod() const {
		return durationOfOnPeriod;
	}

	void setDurationOfOnPeriod(int durationOfOnPeriod) {
		this->durationOfOnPeriod = durationOfOnPeriod;
	}

	int getGreenLed() const {
		return greenLed;
	}

	void setGreenLed(int greenLed) {
		this->greenLed = greenLed;
	}

	int getRedLedOn() const {
		return redLedOn;
	}

	void setRedLedOn(int redLedOn) {
		this->redLedOn = redLedOn;
	}



};

#endif /* INPUTSIGNAL_H_ */
