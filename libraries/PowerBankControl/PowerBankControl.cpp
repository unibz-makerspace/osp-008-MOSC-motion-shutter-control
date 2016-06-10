/*
 * Source code of the library to keep alive a battery power bank.
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

#include "PowerBankControl.h"

#define USING_STATES static uint8_t state = 0;   \
                     static uint32_t tStart = 0; \
                     static bool tFirst = true;

#define STATE(N) if((N == state) && (state++, true))

#define GOTO(N)  state = N

#define DELAY(T) if(tFirst) {                  \
                     tStart = millis();        \
                     tFirst = false;           \
                 }                             \
                 if((millis() - tStart) < T) { \
                     state--;                  \
                 } else {                      \
                     tFirst = true;            \
                 }

static uint8_t _batPin;

void PowerBankControl::begin(uint8_t batPin) {
	_batPin = batPin;
	pinMode(_batPin, OUTPUT);
	digitalWrite(_batPin, LOW);
}

enum BatteryConfiguration {
	BATTERY_DURATION_LOAD_OFF = 1400, //!< milliseconds.
	BATTERY_DURATION_LOAD_ON = 100    //!< milliseconds.
};

enum BatteryStates {
	BATTERY_LOAD_SLEEP,
	BATTERY_LOAD_ON,
	BATTERY_LOAD_OFF
};

void PowerBankControl::keepBatteryAlive(void) {
	USING_STATES;
	STATE(BATTERY_LOAD_SLEEP) {
		DELAY(BATTERY_DURATION_LOAD_OFF);
		return;
	}
	STATE(BATTERY_LOAD_ON) {
		digitalWrite(_batPin, HIGH);
		DELAY(BATTERY_DURATION_LOAD_ON);
		return;
	}
	STATE(BATTERY_LOAD_OFF) {
		digitalWrite(_batPin, LOW);
		GOTO(BATTERY_LOAD_SLEEP);
		return;
	}
}