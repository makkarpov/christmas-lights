#ifndef _EFFECT_RUNNING_LIGHTS_H_
#define _EFFECT_RUNNING_LIGHTS_H_

#include "effect.h"
#include "utils.h"

class EffectRunningLights: public Effect {
	enum {
		BATCH_SIZE = 5,
		BATCH_SPACE = 0,
		
		BATCH_TOTAL = BATCH_SIZE + BATCH_SPACE,
		NUM_BATCHES = LED_STRIP_MAX_COUNT / (BATCH_SIZE + BATCH_SPACE) + 2
	};
	
	enum Mode {
		MODE_RANDOM, MODE_SEQUENTIAL,
		
		MODE_MAXVAL
	};
	
	struct light_state {
		uint16_t hue;
	};
	
	light_state states[NUM_BATCHES];
	uint8_t offset;
	uint16_t shiftTicks, shiftSpeed;
	
	Mode mode;
	uint8_t nextColor;
	
public:
	EffectRunningLights(Leds &_leds);

	virtual void update();
	virtual void buttonClick();
};

#endif // _EFFECT_RUNNING_LIGHTS_H_
