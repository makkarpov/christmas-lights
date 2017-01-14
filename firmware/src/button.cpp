#include "button.h"
#include "config.h"

Button::Button(GPIO_TypeDef *_gpio, uint16_t _pin, bool _invert): gpio(_gpio), pin(_pin), invert(_invert) {
	GPIO_InitTypeDef init;
	init.GPIO_Pin = pin;
	init.GPIO_Mode = invert ? GPIO_Mode_IPU : GPIO_Mode_IPD;
	init.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(gpio, &init);
}

ButtonState Button::state() {
	bool btn = GPIO_ReadInputDataBit(gpio, pin) ^ invert;
	
	// debounce timer:
	if (lastChange > 0) {
		lastChange--;
	}
	
	// time from last event:
	if (idleTime != ((uint16_t) -1)) {
		idleTime++;
	}
	
	// button is held long enough -> report event
	if (longClicks == 0 && prevState && (idleTime > BUTTON_T_LONG_CLICK)) {
		longClicks = 1;
		return BUTTON_LONGCLICK;
	}
	
	// series of clicks, idle time passed -> report event
	if (clicks != 0 && !prevState && (idleTime > BUTTON_T_IDLE)) {
		ButtonState ret = (clicks > 1) ? BUTTON_DBLCLICK : BUTTON_CLICK;
		clicks = 0;
		longClicks = 0;
		return ret;
	}
	
	if (btn != prevState && lastChange == 0) {
		// debounce:
		lastChange = BUTTON_T_DEBOUNCE;
		
		if (!btn) {
			if (longClicks != 0) {
				// button just released after long click, no need to wait for idle time
				longClicks = 0;
				clicks = 0;
			} else {
				// button released after click
				if (idleTime > BUTTON_T_CLICK) {
					clicks++;
				}
			}
		}
		
		prevState = btn;
		idleTime = 0;
	}
	
	return BUTTON_IDLE;
}
