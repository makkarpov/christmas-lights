#ifndef _EFFECT_RANDOM_WIPE_H_
#define _EFFECT_RANDOM_WIPE_H_

#include "effect.h"

class EffectRandomWipe: public Effect {
	enum {
		NUM_WIPES = 3
	};
	
	enum Mode {
		MODE_ALLCOLORS, MODE_6COLOR, MODE_3COLOR,
		
		MODE_MAXVAL
	};
	
	struct wipe_state {
		uint16_t hue;
		int16_t position, spread;
	};
	
	wipe_state wipes[NUM_WIPES];
	uint16_t spreadTicks, spreadSpeed;
	Mode mode;
	
	uint16_t nextHue();
	
public:
	EffectRandomWipe(Leds &_leds);

	virtual void update();
	virtual void buttonClick();
	virtual void resumed();
};

#endif // _EFFECT_RANDOM_WIPE_H_
