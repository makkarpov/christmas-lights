#include "effect_running_lights.h"
#include <stdlib.h>

EffectRunningLights::EffectRunningLights(Leds &_leds): Effect(_leds) {
	offset = 0;
	
	for (uint16_t i = 0; i < NUM_BATCHES; i++) {
		states[i].hue = (6 - i % 6) * 60;
	}
	
	shiftTicks = 0;
	shiftSpeed = 100;
	mode = MODE_RANDOM;
	nextColor = 0;
}

void EffectRunningLights::update() {
	if (++shiftTicks >= shiftSpeed) {
		for (int i = 0; i < leds.count(); i++) {
			int batch, ofs;
			
			if (i < offset) {
				batch = 0;
				ofs = BATCH_TOTAL - offset + i;
			} else {
				batch = (offset == 0 ? 0 : 1) + (i - offset) / BATCH_TOTAL;
				ofs = (i - offset) % BATCH_TOTAL;
			}
			
			if (ofs < BATCH_SIZE) {
				leds.hsv(i, states[batch].hue, 255, 255);
			} else {
				leds.rgb(i, 0, 0, 0);
			}
		}
		
		if (offset == 0) {
			for (uint16_t i = NUM_BATCHES - 1; i > 0; i--) {
				states[i] = states[i - 1];
			}
			
			if (mode == MODE_RANDOM) {
				states[0].hue = rand() % 360;
			} else {
				states[0].hue = nextColor * 60;
				nextColor = (nextColor + 1) % 60;
			}
		}
		
		offset = (offset + 1) % BATCH_TOTAL;
		
		shiftTicks = 0;
	}
}

void EffectRunningLights::buttonClick() {
	mode = (Mode) ((mode + 1) % MODE_MAXVAL);
}
