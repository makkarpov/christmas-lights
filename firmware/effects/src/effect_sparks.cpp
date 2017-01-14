#include "effect_sparks.h"
#include <stdlib.h>

EffectSparks::EffectSparks(Leds &_leds): Effect(_leds) {
	for (uint16_t i = 0; i < LED_STRIP_MAX_COUNT; i++) {
		sparks[i].value = 0;
	}
	
	fadeSpeed = 10;
	fadeStep = 3;
	respawnSpeed = 20;
	
	fadeTicks = 0;
	respawnTicks = 0;
	
	mode = MODE_STEPS;
}

void EffectSparks::update() {
	// Fade out & display sparks:
	if (++respawnTicks >= respawnSpeed) {
		for (uint16_t i = 0; i < LED_STRIP_MAX_COUNT; i++) {
			if (sparks[i].value > fadeStep) {
				sparks[i].value -= fadeStep;
			} else {
				sparks[i].value = 0;
			}
			
			leds.hsv(i, sparks[i].hue, 255, sparks[i].value);
		}
		
		respawnTicks = 0;
	}
	
	// Randomly create new sparks:
	if (++fadeTicks > fadeSpeed) {
		for (uint8_t i = 0; i < 3; i++) {
			uint16_t idx = rand() % leds.count();
			
			sparks[idx].value = 255;
			sparks[idx].hue = (mode == MODE_RANDOM) ? (rand() % 360) : hue;
		}
		
		if (mode == MODE_RAINBOW) {
			hue++;
		} else if (mode == MODE_STEPS) {
			if (nextStep == 0) {
				hue = (hue / 60) * 60;
				hue = (hue + 60) % 360;
				nextStep = 100;
			} else {
				nextStep--;
			}
		}
		
		fadeTicks = 0;
	}
}

void EffectSparks::buttonClick() {
	mode = (Mode) ((mode + 1) % MODE_MAXVAL);
}
