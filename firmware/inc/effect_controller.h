#ifndef _EFFECT_CONTROLLER_H_
#define _EFFECT_CONTROLLER_H_

#include "leds.h"
#include "button.h"
#include "effect.h"
#include "effect_descriptor.h"

class EffectController {
	Leds &leds;
	Button button;
	const EffectDescriptor *effects;
	
	bool is_on;
	int effect_idx;
	Effect *effect;
	
public:
	EffectController(Leds &_leds, const EffectDescriptor *_effects);
	void tick();
};

#endif // _EFFECT_CONTROLLER_H_
