#ifndef _ALL_EFFECTS_H_
#define _ALL_EFFECTS_H_

#include "effect_rainbow.h"
#include "effect_sparks.h"
#include "effect_running_lights.h"
#include "effect_random_wipe.h"
#include "effect_flicker1.h"

#define ALL_EFFECTS_DESCRIPTOR EFFECT_DESCRIPTOR( \
	EffectRainbow, EffectSparks, EffectRunningLights, EffectRandomWipe, EffectFlicker1 \
	)

#endif // _ALL_EFFECTS_H_
