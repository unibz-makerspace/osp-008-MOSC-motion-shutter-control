/*
 * Source code of the wireless motion controlled camera shutter for Faculty of
 * Design student Vincent Castonguay.
 *
 * A project in collaboration with makerspace - Faculty of Computer Science 
 * at the Free University of Bozen-Bolzano.
 * 
 *                                                                         
 *    m  a  k  e  r  s  p  a  c  e  .  i  n  f  .  u  n  i  b  z  .  i  t  
 *                                                                         
 *   8888888888888888888888888888888888888888888888888888888888888888888888
 *                                                                         
 *                  8                                                      
 *                  8                                                      
 *   YoYoYo. .oPYo. 8  .o  .oPYo. YoYo. .oPYo. 8oPYo. .oPYo. .oPYo. .oPYo. 
 *   8' 8' 8 .oooo8 8oP'   8oooo8 8  `  Yb..`  8    8 .oooo8 8   `  8oooo8 
 *   8  8  8 8    8 8 `b.  8.  .  8      .'Yb. 8    8 8    8 8   .  8.  .  
 *   8  8  8 `YooP8 8  `o. `Yooo' 8     `YooP' 8YooP' `YooP8 `YooP' `Yooo' 
 *                                             8                           
 *                                             8                           
 *                                                                         
 *   8888888888888888888888888888888888888888888888888888888888888888888888
 *                                                                         
 *    c  o  m  p  u  t  e  r    s  c  i  e  n  c  e    f  a  c  u  l  t  y 
 *                                                                         
 *                                                                         
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Julian Sanin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef _MOTIONSTATION_h
#define _MOTIONSTATION_h

#include <Arduino.h>

#include <stdint.h>
#include <string.h>

#include <RH_ASK.h>

namespace RadioConstants {
	enum {
		//! Send at least every second a presence signal.
		HEARTBEAT_DURATION = 1000 // milliseconds.
	};
}

template <
	const uint8_t pirPin, //!< Motion sensor pin, active when high.
	const uint8_t pttPin, //!< PTT pin. Not used here. Don't connect anything.
	const uint8_t rxPin,  //!< Rx pin. Not used here. Don't connect anything.
	const uint8_t txPin,  //!< Wireless 433MHz radio transmitter pin.
	const uint8_t actPin  //!< Status LED pin. set to high when transmitting
>                         //!< the motion detected signal.
class MotionStation {

	RH_ASK radioTransmitter;
	bool previousPirSignal;
	bool isMotionDetected;
	uint32_t radioOffDuration;
	uint32_t timerStartTime;

	void checkMotionSignal(void) {
		bool currentPirSignal = digitalRead(pirPin);
		// Accept only a low to high edge as motion detected signal.
		if ((previousPirSignal == LOW) && (currentPirSignal == HIGH)) {
			isMotionDetected = true;
		}
		previousPirSignal = currentPirSignal;
	}

	void sendMotionSignal(void) {
		if (this->isMotionDetected == true) {
			const char * motionSignalDetected = "1";
			digitalWrite(actPin, HIGH);
			radioTransmitter.send(
				(uint8_t *)(motionSignalDetected),
				strlen(motionSignalDetected)
			);
			radioTransmitter.waitPacketSent();
			digitalWrite(actPin, LOW);
			this->isMotionDetected = false;
		}
		// Switch off the radio at least for a given amount of time to respect
		// the 10% on duty cycle, as required by the wireless regulations for
		// ISM bands.
		if (timerStartTime == 0) {
			timerStartTime = millis();
		}
		if (millis() - timerStartTime >= radioOffDuration) {
			const char * motionSignal = "0";
			radioTransmitter.send((uint8_t *)(motionSignal), strlen(motionSignal));
			radioTransmitter.waitPacketSent();
			timerStartTime = 0;
		}
	}

public:

	MotionStation() :
		previousPirSignal(LOW),  //!< Motion detector signals low when idle.
		isMotionDetected(false), //!< Only true when low to high edge is found. 
		radioOffDuration(0),
		timerStartTime(0) {
	}

	void begin(uint16_t speedInBitsPerSecond) {
		radioTransmitter = RH_ASK(speedInBitsPerSecond, rxPin, txPin, pttPin);
		radioTransmitter.init();
		pinMode(actPin, OUTPUT);
		pinMode(pirPin, INPUT); //!< with pulldown resistor.
		radioOffDuration = max(
			2/* maximum bytes */ *
			8/* bits per byte */ *
			10000/* 10%:100% off ratio in milliseconds */ /
			speedInBitsPerSecond,
			RadioConstants::HEARTBEAT_DURATION);
	}

	void run(void) {
		checkMotionSignal();
		sendMotionSignal();
	}
};

#endif

