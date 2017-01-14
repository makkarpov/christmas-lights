#include "usb_handler.h"
#include <stm32f10x.h>
#include <usb_lib.h>
#include "config.h"

#define DEVICE_UID 			((uint32_t*) 0x1FFFF7E8)

uint8_t receivedData[ISOC_EP_PACKET_SIZE];

void USB_Initialize() {
  GPIO_InitTypeDef gpio;
  EXTI_InitTypeDef exti;
  NVIC_InitTypeDef nvic;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD, ENABLE);
  
  gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  gpio.GPIO_Mode = GPIO_Mode_Out_PP;
  gpio.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &gpio);
  
  /* GPIOD2 used as USB pull-up */
  gpio.GPIO_Pin = GPIO_Pin_2;
  gpio.GPIO_Speed = GPIO_Speed_50MHz;
  gpio.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOD, &gpio);

  /* Configure the EXTI line 18 connected internally to the USB IP */
  EXTI_ClearITPendingBit(EXTI_Line18);
  exti.EXTI_Line = EXTI_Line18; 
  exti.EXTI_Mode = EXTI_Mode_Interrupt;
  exti.EXTI_Trigger = EXTI_Trigger_Rising;
  exti.EXTI_LineCmd = ENABLE;
  EXTI_Init(&exti); 
  
  /* 2 bit for pre-emption priority, 2 bits for subpriority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
  
  /* Enable the USB interrupt */
  nvic.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  nvic.NVIC_IRQChannelPreemptionPriority = 1;
  nvic.NVIC_IRQChannelSubPriority = 0;
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);
  
  /* Enable the USB Wake-up interrupt */
  nvic.NVIC_IRQChannel = USBWakeUp_IRQn;
  nvic.NVIC_IRQChannelPreemptionPriority = 0;
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);
  
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
  
  USB_Init();
}

void USB_PullupConfig(char state) {
	GPIO_WriteBit(GPIOD, GPIO_Pin_2, state ? Bit_RESET : Bit_SET);
}

void USB_PopulateSerialNumber() {
	uint32_t hash = 0xFEEDF00D;
	
	// Hash device unique IDs
	hash = DEVICE_UID[0] * 1664525 + 1013904223;
	hash = hash * 1664525 + 1013904223;
	hash = DEVICE_UID[1] * 1664525 + 1013904223;
	hash = hash * 1664525 + 1013904223;
	hash = DEVICE_UID[2] * 1664525 + 1013904223;
	hash = hash * 1664525 + 1013904223;
	hash = hash * 1664525 + 1013904223;
	hash = hash * 1664525 + 1013904223;
	
	// Write hash in HEX form
	for (int i = 10; i < 26; i += 2) {
		uint8_t digit = hash >> 28;
		
		if (digit < 10) {
			CustomHID_StringSerial[i] = '0' + digit;
		} else {
			CustomHID_StringSerial[i] = 'a' + digit - 10;
		}
		
		CustomHID_StringSerial[i + 1] = 0;
		hash <<= 4;
	}
}

void EP1_IN_Callback() {
	int length = USB_SIL_Read(EP1_OUT, receivedData);
	
  SetEPRxStatus(ENDP1, EP_RX_VALID);
}

void EP1_OUT_Callback() {
}

void EP2_OUT_Callback() {
		uint16_t length;
	if (GetENDPOINT(ENDP2) & EP_DTOG_TX) {
		length = GetEPDblBuf0Count(ENDP2);
		PMAToUserBufferCopy(receivedData, ENDP2_RXBUF1, length);
	} else {
		length = GetEPDblBuf1Count(ENDP2);
		PMAToUserBufferCopy(receivedData, ENDP2_RXBUF2, length);
	}
	
	FreeUserBuffer(ENDP2, EP_DBUF_OUT);
}
