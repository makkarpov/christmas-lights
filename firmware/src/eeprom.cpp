#include "eeprom.h"

#include <stm32f10x.h>

/**
  * Current EEPROM mapping (pages):
	*   0 ... 7:   global configuration
	*   8 ... 511: currently unused
	*/

EEPROM eeprom;

EEPROM::EEPROM() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &gpio);
	
	I2C_InitTypeDef i2c;
	i2c.I2C_ClockSpeed = 100000;
	i2c.I2C_Mode = I2C_Mode_I2C;
	i2c.I2C_DutyCycle = I2C_DutyCycle_2;
	i2c.I2C_OwnAddress1 = 0x01;
	i2c.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	i2c.I2C_Ack = I2C_Ack_Enable;
	
	I2C_Cmd(I2C1, ENABLE);
	I2C_Init(I2C1, &i2c);
	
	hwAddr = 0xA0;
}

uint8_t EEPROM::read(uint16_t addr) {
	uint8_t ret;
	read(addr, &ret, sizeof(ret));
	return ret;
}

void EEPROM::write(uint16_t addr, uint8_t data) {
	write(addr, &data, sizeof(data));
}

void EEPROM::read(uint16_t addr, uint8_t *buf, uint16_t len) {
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	
	I2C_GenerateSTART(I2C1, ENABLE);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(I2C1, hwAddr, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	
	I2C_SendData(I2C1, addr >> 8);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_SendData(I2C1, addr);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTART(I2C1, ENABLE);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(I2C1, hwAddr, I2C_Direction_Receiver);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
	
	for (uint16_t i = 0; i < len; i++) {
		if (i == len - 1) {
			I2C_AcknowledgeConfig(I2C1, DISABLE);
		}
		
		buf[i] = I2C_ReceiveData(I2C1);
	}
	
	I2C_GenerateSTOP(I2C1, ENABLE);
}

void EEPROM::write(uint16_t addr, uint8_t *buf, uint16_t len) {
	I2C_GenerateSTART(I2C1, ENABLE);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(I2C1, hwAddr, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	
	I2C_SendData(I2C1, addr >> 8);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_SendData(I2C1, addr);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	for (uint16_t i = 0; i < len; i++) {
		I2C_SendData(I2C1, buf[i]);
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	}
	
	I2C_GenerateSTOP(I2C1, ENABLE);
}