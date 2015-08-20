#include <Arduino.h>
#include <roboteqMC.h>

AX2550::AX2550(HardwareSerial& serial_port):uart(serial_port)
{
   uart.begin(9600,SERIAL_7E1);
}

void AX2550::init(uint8_t x,uint8_t y){
   x_key = x;
   y_key = y;
}

void AX2550::set_report(uint8_t v,uint8_t l,uint8_t r){
   volt_key = v;
   amps_left_key = l;
   amps_right_key = r;
}

bool AX2550::move(int Steering,int Throttle, bool check)
{
   int8_t Dir1 = 0;
   int8_t Dir2 = 0;
   while(uart.available()>0)
   {
      uart.read();
   }
   if(Throttle>=0)
   {
      Dir1 = 0x41;
   }
   if(Throttle<0)
   {
      Dir1 = 0x61;
   }
   if(Steering>=0)
   {
      Dir2 = 0x42;
   }
   if(Steering<0)
   {
      Dir2 = 0x62;
   }

   Throttle = map(abs(Throttle),0,100,0,126);
   
   Steering = map(abs(Steering),0,100,0,126);

   char command[12];
   sprintf(command,"!%c%02x\r\n!%c%02x\r\n",Dir1,abs(Throttle),Dir2,abs(Steering));
   uart.print(command);

   if(check){
      delay(10);

      bool chk_throttle = chkResponse();
      bool chk_steering = chkResponse();

      while(uart.available()>0){
         uart.read();
      }
      return chk_throttle & chk_steering;
   }

   return true;
}

bool AX2550::move(Message inp, bool check){
   if(inp.new_data(y_key) || inp.new_data(x_key)){
      return move(int16_t(inp.get_data(y_key)),int16_t(inp.get_data(x_key)),check);
   }
   return false;
}

bool AX2550::chkResponse()
{
   char status;
   uint8_t counter=0;
   while(1)
   {
      status = uart.read();
      //Serial.print(status);
      if(status == '+')
      {
         return true;
      }
      else if(status == '-')
      {
         //Serial.println("Command Fails!");
         return false;
      }
      else if(counter == 20)
      {
         Serial3.println("Chk response Timeout");
         return false;
      }
      else
      {
         counter+=1;
         delay(1);
      }
   }
}

float AX2550::volt()
{
   while(uart.available())
   {
      uart.read();
   }
   char command[2];
   sprintf(command,"?e");
   if(sendChk(command))
   {
      char inp[2];
      
      uart.read();
      uart.read();
      uart.read();
      uart.read();
      
      inp[0] = '0';
      inp[1] = 'x';
      inp[2] = char(uart.read());
      inp[3] = char(uart.read());

      // Serial.println(inp[0]);
      // Serial.println(inp[1]);
      // Serial.println(inp[2]);
      // Serial.println(inp[3]);

      float volt = 28.5 * strtoul(inp,0,16)/256;

      // Serial.println(volt);

      delay(10);

      while(uart.available())
      {
         uart.read();
      }

      return volt;
   }
   return 0.0;
}

int AX2550::amps(uint8_t channel)
{
   while(uart.available())
   {
      uart.read();
   }

   char command[2];
   sprintf(command,"?a");
   
   if(sendChk(command))
   {
      char inp[2];
      
     if(channel == 1)
     {
         uart.read();
      
         inp[0] = '0';
         inp[1] = 'x';
         inp[2] = char(uart.read());
         inp[3] = char(uart.read());
      }

     if(channel == 2)
     {
         uart.read();
         uart.read();
         uart.read();
         uart.read();
      
         inp[0] = '0';
         inp[1] = 'x';
         inp[2] = char(uart.read());
         inp[3] = char(uart.read());
      }


      // Serial.println(inp[0]);
      // Serial.println(inp[1]);
      // Serial.println(inp[2]);
      // Serial.println(inp[3]);

      int amps = strtoul(inp,0,16);

      // Serial.println(volt);

      delay(10);

      while(uart.available())
      {
         uart.read();
      }
      return amps;
   }
   // Serial.println("Fail!");
   return 0;
}

bool AX2550::sendChk(char buff[])
{
   uint8_t lenght = strlen(buff);
   char input_buff[lenght];
   uart.println(buff);

   uint8_t counter=0;
   uint8_t timeout=100;

   delay(10);

   while(1)
   {
      if(uart.available()>0)
      {
         for(int i=0;i<lenght;i++)
         {
            input_buff[i]=uart.read();
            if(input_buff[i]!=buff[i])
            {
               //Echo Condition
               // Serial.println("Echo Error!");
               return false;
            }
         }
         return true;
      }
      else if(counter == timeout)
      {
         //Timeout condition
         // Serial.println("Timeout Error!");
         return false;
      }
      else
      {
         counter+=1;
         delay(1);
      }
   }
}

Message AX2550::report(){
   Message output;
   output.add_data(amps_left_key,uint16_t(amps(1)));
   output.add_data(amps_right_key,uint16_t(amps(2)));
   output.add_data(volt_key,uint16_t(volt()));
   return output;
}