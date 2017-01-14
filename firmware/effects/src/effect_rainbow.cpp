#include "effect_rainbow.h"

EffectRainbow::EffectRainbow(Leds &_leds): Effect(_leds) {
	hue = 0;
	ticks = 0;
	mode = false;
}

void EffectRainbow::update() {
	if (++ticks >= 100) {
		int h = hue;
		
		for (uint32_t i = 0; i < leds.count(); i++) {
			leds.hsv(i, h, 255, 255);
			if (mode) {
				h = (h + 5) % 360;
			}
		}
		
		leds.requestUpdate();
		hue = (hue + 1) % 360;
		ticks = 0;
	}
}

void EffectRainbow::buttonClick() {
	mode = !mode;
	ticks = 0;
}
