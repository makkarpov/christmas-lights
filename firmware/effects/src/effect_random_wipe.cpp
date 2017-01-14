#include "effect_random_wipe.h"
#include <stdlib.h>

EffectRandomWipe::EffectRandomWipe(Leds &_leds): Effect(_leds) {
	spreadTicks = 0;
	spreadSpeed = 20;
	mode = MODE_ALLCOLORS;
	
	resumed();
}

uint16_t EffectRandomWipe::nextHue() {
	switch (mode) {
		case MODE_ALLCOLORS: return rand() % 360;
		case MODE_6COLOR: return (rand() % 6) * 60;
		case MODE_3COLOR: return (rand() % 3) * 120;
	}
}

void EffectRandomWipe::update() {
	if (++spreadTicks >= spreadSpeed) {
		for (int i = 0; i < NUM_WIPES; i++) {
			int idx = wipes[i].position - wipes[i].spread;
			if (idx >= 0) {
				leds.hsv(idx, wipes[i].hue, 255, 255);
			}
			
			idx = wipes[i].position + wipes[i].spread;
			if (idx < leds.count()) {
				leds.hsv(idx, wipes[i].hue, 255, 255);
			}
			
			wipes[i].spread++;
			
			bool shouldReplace = true;
			
			shouldReplace &= (wipes[i].position - wipes[i].spread) < 0;
			shouldReplace &= (wipes[i].position + wipes[i].spread) >= leds.count();
			
			if (shouldReplace) {
				wipes[i].hue = nextHue();
				wipes[i].position = rand() % leds.count();
				wipes[i].spread = 1;
			}
		}
		
		spreadTicks = 0;
	}
}

void EffectRandomWipe::buttonClick() {
	mode = (Mode) ((mode + 1) % MODE_MAXVAL);
}

void EffectRandomWipe::resumed() {
	for (int i = 0; i < leds.count(); i++) {
		leds.rgb(i, 0, 0, 0);
	}
	
	for (int i = 0; i < NUM_WIPES; i++) {
		wipes[i].hue = nextHue();
		wipes[i].position = rand() % leds.count();
		wipes[i].spread = 1;
	}
}
