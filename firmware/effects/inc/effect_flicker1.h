#ifndef _EFFECT_FLICKER1_H_
#define _EFFECT_FLICKER1_H_

#include "effect.h"

class EffectFlicker1: public Effect {
	enum {
		FLICKER_COUNT			= 3,
		FLICKER_REPEATS 	= 2,
		
		PHASE_LED_PERIOD    = 2 * 255,
		PHASE_COUNT_PERIOD  = PHASE_LED_PERIOD * FLICKER_COUNT,
		PHASE_REPEAT_PERIOD = PHASE_COUNT_PERIOD * 2,
		PHASE_PERIOD        = PHASE_REPEAT_PERIOD * FLICKER_REPEATS
	};
	
	uint32_t flickerPhase;
	uint16_t hue1, hue2;
	
	uint8_t flickerTicks, flickerSpeed;
	
public:
	EffectFlicker1(Leds &_leds);

	virtual void update();
	virtual void buttonClick();
};

#endif // _EFFECT_FLICKER1_H_
