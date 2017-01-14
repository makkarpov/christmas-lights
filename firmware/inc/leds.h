#ifndef _LEDS_H_
#define _LEDS_H_

#include "ws2812b.h"
#include "config.h"

extern "C" void ws2812b_DMAFinished();

class Leds {
#if LED_STRIP_DOUBLE_BUFFER == 1
	RGB_t leds_buffer[2][LED_STRIP_MAX_COUNT];
	volatile uint8_t current_buffer;
#else
	RGB_t leds_buffer[LED_STRIP_MAX_COUNT];
#endif
	volatile bool update_requested, xfer_pending;

public:
	Leds();

	int count() const;
	RGB_t &operator[](uint32_t i);
	void hsv(uint32_t i, uint16_t h, uint8_t s, uint8_t v);
	void rgb(uint32_t i, uint8_t r, uint8_t g, uint8_t b);

	void requestUpdate();
	void forceUpdate();
	void effectTick();

	friend void ws2812b_DMAFinished();
};

extern Leds leds;

#endif // _LEDS_H_
