#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "leds.h"
#include "effect.h"
#include <cstddef>

using std::size_t;

class Effect {
protected:
	Leds &leds;

public:
	Effect(Leds &_leds);
	virtual ~Effect();

	virtual void update();
	virtual void buttonClick();

	virtual void resumed();

	static void *operator new(size_t size) throw();
};

extern uint8_t effectBuffer[];

#endif // _EFFECT_H_
