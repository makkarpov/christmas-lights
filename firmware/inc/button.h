#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <stm32f10x.h>

enum ButtonState {
	BUTTON_IDLE,
	BUTTON_CLICK,
	BUTTON_DBLCLICK,
	BUTTON_LONGCLICK
};

/* Button debounce handler */
class Button {
	GPIO_TypeDef *gpio;
	uint16_t pin;
	bool invert;
	
	// FSM:
	bool prevState;
	uint16_t lastChange, idleTime;
	uint8_t clicks, longClicks;
	
public:
	Button(GPIO_TypeDef *_gpio, uint16_t _pin, bool _invert);
	ButtonState state();
};

#endif // _BUTTON_H_
