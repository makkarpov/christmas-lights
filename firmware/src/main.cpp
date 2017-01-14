#include "usb_handler.h"
#include <stm32f10x.h>
#include <stdlib.h>

#include "effect_controller.h"
#include "effect_descriptor.h"
#include "effect.h"
#include "eeprom.h"
#include "config.h"
#include "all_effects.h"

volatile int ticks = 0;

extern "C" void SysTick_Handler() {
	if (ticks > 0) {
		ticks--;
	}
}

// seed random from ADC noise
static void seedRandom() {
	unsigned int seed = 0;
	
	// turn on ADC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	ADC_InitTypeDef adc;
	ADC_StructInit(&adc);
	adc.ADC_Mode = ADC_Mode_Independent;
	adc.ADC_ScanConvMode = DISABLE;
	adc.ADC_ContinuousConvMode = DISABLE;
	adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	adc.ADC_DataAlign = ADC_DataAlign_Right;
	adc.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &adc);
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);
	
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1)); 
	
	// measure 100 samples and hash them:
	
	for (int i = 0; i < 100; i++) {
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
		uint16_t conv = ADC_GetConversionValue(ADC1);
		
		seed = (seed * 1664525) + 1013904223 + conv;
		seed = (seed * 1664525) + 1013904223 + conv;
		seed = (seed * 1664525) + 1013904223 + conv;
		seed = (seed * 1664525) + 1013904223 + conv;
		
		ticks = 1;
		while (ticks) __wfi();
	}
	
	srand(seed);
}

int main() {
	USB_Initialize();
	SysTick_Config((SystemCoreClock / 1000) - 1);
	
	seedRandom();
	
	EffectController controller(leds, ALL_EFFECTS_DESCRIPTOR);
	
	while (1) {
		controller.tick();
		ticks = 1;
		while (ticks) __wfi();
	}
}
