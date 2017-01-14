#include "effect_flicker1.h"

#include <stdlib.h>

EffectFlicker1::EffectFlicker1(Leds &_leds): Effect(_leds) {
	flickerPhase = 0;
	hue1 = 0;
	hue2 = 120;
	
	flickerTicks = 0;
	flickerSpeed = 3;
}

void EffectFlicker1::update() {
	if (++flickerTicks >= flickerSpeed) {
		uint8_t v1 = 0, v2 = 0, v;
		uint32_t ledPhase = flickerPhase % PHASE_LED_PERIOD;
		
		if (ledPhase <= 255) {
			v = ledPhase;
		} else {
			v = 255 - ledPhase;
		}
		
		// Which led is currently flashing?
		if ((flickerPhase / PHASE_COUNT_PERIOD) % 2 == 0) {
			v1 = v;
		} else {
			v2 = v;
		}
		
		for (int i = 0; i < leds.count(); i++) {
			if (i % 2 == 0) {
				leds.hsv(i, hue1, 255, v1);
			} else {
				leds.hsv(i, hue2, 255, v2);
			}
		}
		
		flickerPhase = flickerPhase + 6;
		
		if (flickerPhase >= PHASE_PERIOD) {
			flickerPhase %= PHASE_PERIOD;
			hue1 = rand() % 360;
			hue2 = (hue1 + 180) % 360;
		}
		
		flickerTicks = 0;
	}
}

void EffectFlicker1::buttonClick() {
}
