#include "leds.h"

Leds leds;

Leds::Leds() {
#if LED_STRIP_DOUBLE_BUFFER == 1
	current_buffer = 0;
	for (uint8_t buf = 0; buf < 2; buf++) {
		for (uint32_t i = 0; i < LED_STRIP_MAX_COUNT; i++) {
			leds_buffer[buf][i].r = 0;
			leds_buffer[buf][i].g = 0;
			leds_buffer[buf][i].b = 0;
		}
	}
#else
	for (uint32_t i = 0; i < LED_STRIP_MAX_COUNT; i++) {
		leds_buffer[i].r = 0;
		leds_buffer[i].g = 0;
		leds_buffer[i].b = 0;
	}
#endif
	
	update_requested = false;
	xfer_pending = false;
	ws2812b_Init();
}

int Leds::count() const {
	return LED_STRIP_MAX_COUNT;
}

RGB_t &Leds::operator[](uint32_t i) {
#if LED_STRIP_DOUBLE_BUFFER == 1
	return leds_buffer[current_buffer][i];
#else
	return leds_buffer[i];
#endif
}

void Leds::hsv(uint32_t i, uint16_t h, uint8_t s, uint8_t v) {
	HSV_t hsv;
	hsv.h = h;
	hsv.s = s;
	hsv.v = v;
	HSV2RGB(&hsv, &operator[](i));
}

void Leds::rgb(uint32_t i, uint8_t r, uint8_t g, uint8_t b) {
	(*this)[i].r = r;
	(*this)[i].g = g;
	(*this)[i].b = b;
}

void Leds::requestUpdate() {
	update_requested = true;
}

void Leds::forceUpdate() {
	if (ws2812b_IsReady()) {
		update_requested = false;
		xfer_pending = false;
#if LED_STRIP_DOUBLE_BUFFER == 1
		ws2812b_SendRGB(leds_buffer[current_buffer], count());
		current_buffer = 1 - current_buffer;
#else
		ws2812b_SendRGB(leds_buffer, count());
#endif
	} else {
		xfer_pending = true;
	}
}

void Leds::effectTick() {
	if (update_requested) {
		forceUpdate();
	}
}

extern "C" void ws2812b_DMAFinished() {
	if (leds.xfer_pending) {
		leds.forceUpdate();
	}
}
