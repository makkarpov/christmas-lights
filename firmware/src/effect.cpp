#include "effect.h"

uint8_t effectBuffer[EFFECT_DATA_BUFFER];

// allocator:
void *Effect::operator new(size_t size) throw() {
	if (size <= EFFECT_DATA_BUFFER) {
		return effectBuffer;
	} else {
		return 0;
	}
}

Effect::Effect(Leds &_leds): leds(_leds) {
}

Effect::~Effect() {
}

void Effect::update() {
}

void Effect::buttonClick() {
}

void Effect::resumed() {
}
