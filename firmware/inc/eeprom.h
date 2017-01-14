#ifndef _EEPROM_H_
#define _EEPROM_H_

#include <stm32f10x.h>

class EEPROM {
	uint8_t hwAddr;
	
public:
	EEPROM();

	uint8_t read(uint16_t addr);
	void write(uint16_t addr, uint8_t data);

	void read(uint16_t addr, uint8_t *buf, uint16_t len);
	void write(uint16_t addr, uint8_t *buf, uint16_t len);
};

extern EEPROM eeprom;

#endif // _EEPROM_H_
