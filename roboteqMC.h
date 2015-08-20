#ifndef roboteqMC_h
#define roboteqMC_h

#include <Arduino.h>
#include "../Ard-MSG/Message.h"

class AX2550
{
public:
	AX2550(HardwareSerial&);
	void init(uint8_t,uint8_t);
	void set_report(uint8_t,uint8_t,uint8_t);
	bool move(int,int,bool);
	bool move(Message input,bool);
	float volt();
	int amps(uint8_t);
	Message report();
private:
	HardwareSerial& uart;
	bool chkResponse();
	bool sendChk(char[]);
	uint8_t x_key;
	uint8_t y_key;
	uint8_t volt_key;
	uint8_t amps_left_key;
	uint8_t amps_right_key;
};

#endif