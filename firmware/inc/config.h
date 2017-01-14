#ifndef _CONFIG_H_
#define _CONFIG_H_

//*** <<< Use Configuration Wizard in Context Menu >>> ***

// <h> LED strip configuration

//   <o> LED wiring  <0=> RGB <1=> GRB
//   <i> How chip outputs are wired to actual LED colors.
//   <i> Affects color order in bit stream.
#define LED_STRIP_WIRING 						0

//   <o> Maximum LED count
//   <i> Actual LED count can be smaller than maximum settings.
//   <i> Affects memory allocation for buffers.
#define LED_STRIP_MAX_COUNT 				100

//   <q> Use double buffering
#define LED_STRIP_DOUBLE_BUFFER 		0

//   <q> Use gamma correction
#define LED_STRIP_GAMMA_CORRECTION  1

//   <o> DMA bufffer size <5-100>
#define LED_STRIP_BUFFER_SIZE       30

// </h>

// <h> Button handling

///  <o> T_DEBOUNCE (ms)
#define BUTTON_T_DEBOUNCE 					30

//   <o> T_CLICK (ms)
//   <i> Button has to be held for that time for click to be registered
#define BUTTON_T_CLICK							100

//   <o> T_LONG_CLICK (ms)
//   <i> Button has to be held for that time for long click to be registered
#define BUTTON_T_LONG_CLICK					1000

//   <o> T_IDLE (ms)
//   <i> Time window in which double click could be registered
#define BUTTON_T_IDLE								300

// </h>

// <h> Hardware configuration

//   <q> Use development board
//   <i> This setting will reassign some GPIOs to match development board assignments
#define USE_DEV_BOARD								1

//   <o> Isochronous endpoint packet size
#define ISOC_EP_PACKET_SIZE         64

//   <o> Buffer for effect data
#define EFFECT_DATA_BUFFER					1024

// </h>

#endif // _CONFIG_H_
