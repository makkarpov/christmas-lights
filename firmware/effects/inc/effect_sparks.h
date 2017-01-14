#ifndef _EFFECT_SPARKS_H_
#define _EFFECT_SPARKS_H_

#include "Effect.h"

class EffectSparks: public Effect {
	struct spark_state {
		uint16_t hue;
		uint8_t value;
	} sparks[LED_STRIP_MAX_COUNT];
	
	enum Mode {
		MODE_RAINBOW, MODE_STEPS, MODE_RANDOM,
		
		MODE_MAXVAL
	};
	
	uint8_t fadeSpeed, respawnSpeed;
	uint8_t fadeTicks, respawnTicks;
	uint8_t fadeStep;
	uint16_t hue, nextStep;
	
	Mode mode;
	
public:
	EffectSparks(Leds &_leds);

	virtual void update();
	virtual void buttonClick();
};

#endif // _EFFECT_SPARKS_H_
