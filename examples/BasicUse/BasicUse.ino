#include "roboteqMC.h"
#include "Message.h"

AX2550 trac(Serial1);
Message msg;


void setup(){
	Serial.begin(115200);
	Serial1.begin(9600,SERIAL_7E1);
	trac.init(0x0A,0x0B);
}

void loop(){
	msg.add_data(0x0A,100);
	msg.add_data(0x0B,100);
	
	trac.move(msg,true);

	delay(5000);

	trac.move(0,0,true);

	trac.set_report('V','L','R');

	msg = trac.report();

	msg.debug();

	Serial.print("voltage = ");Serial.println(trac.volt());
	Serial.print("Current 1 ");Serial.println(trac.amps(1));
	Serial.print("Current 2 ");Serial.println(trac.amps(2));

	while(1){
	    Serial.println("Reset to restart!...");
	}
}