#ifndef _EFFECT_RAINBOW_
#define _EFFECT_RAINBOW_

#include "effect.h"

class EffectRainbow: public Effect {
	int hue, ticks;
	bool mode;
	
public:
	EffectRainbow(Leds &_leds);

	virtual void update();
	virtual void buttonClick();
};

#endif // _EFFECT_RAINBOW_
