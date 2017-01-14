#include "effect_controller.h"
#include <stm32f10x.h>

#include "effect_descriptor.h"
#include "eeprom.h"
#include "eeprom_address.h"

EffectController::EffectController(Leds &_leds, const EffectDescriptor *_effects)
	: leds(_leds), button(GPIOA, GPIO_Pin_8, true), effects(_effects)
{
	// TODO: read from EEPROM
	is_on = true;
	effect_idx = eeprom.read(EE_CURRENT_EFFECT) % effects->count();
	
	effect = effects->get(effect_idx)->create(leds);
}

void EffectController::tick() {
	ButtonState btn = button.state();
	
	if (btn == BUTTON_LONGCLICK) {
		is_on = !is_on;
		
		// blank all leds:
		if (!is_on) {
			for (int i = 0; i < leds.count(); i++) {
				leds[i].r = 0;
				leds[i].g = 0;
				leds[i].b = 0;
			}
			
			leds.requestUpdate();
		} else {
			effect->resumed();
		}
	}
	
	if (btn == BUTTON_DBLCLICK && is_on) {
		// switch effect:
		effect->~Effect();
		effect_idx = (effect_idx + 1) % effects->count();
		effect = effects->get(effect_idx)->create(leds);
		
		eeprom.write(EE_CURRENT_EFFECT, effect_idx);
	}
	
	if (btn == BUTTON_CLICK && is_on) {
		effect->buttonClick();
	}
	
	if (is_on) {
		effect->update();
		leds.requestUpdate();
	}
	
	leds.effectTick();
}
