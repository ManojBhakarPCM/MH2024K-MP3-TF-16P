#define DF_RX 11       //Near VCC
#define DF_TX 10       //Far From VCC
#include "Arduino.h"
#include "SoftwareSerial.h" //Module Works With Serial Protocol.
#define VOLUME 0x06
#define SOUNDNO 0x03
#define RESET 0x0C
#define EQ  0x07
#define NEXT  0x01
//---------------------------------------------------------------
SoftwareSerial mySerial(DF_TX, DF_RX);
void setup(){
mySerial.begin (9600);
delay(1000);

DFPlayerSetting(RESET,0,0); //CORRECTION2: always reset the module first, otherwise may arise NO SOUND PROBLEM.
delay(5000);                //CORRECTION3: always give some time to let it initialize before sending next command.

DFPlayerSetting(NEXT,0x00,0x00); //Play first File. this always works even if the first file number is above 1000. because it just "finds" it!!
delay(1000);

DFPlayerSetting(VOLUME, 0, 5);   //CORRECTION4: Set DFPlayer volume (0x00~0x1E) means (0-30)


DFPlayerSetting(0x03,0,3); //play 3rd file.
delay(5000);


DFPlayerSetting(VOLUME, 0, 15); // volume level changed from 5 to 15 shows that volume control is working.
//---------------------------------------------------------------
}
void loop(){
  for(byte i=0;i<15;i++){         //looping first 15 files to test if they can be played by number.
    DFPlayerSetting(0x03,0,i);    //play i-th file in folder no 0
    delay(5000);                  //for 5 seconds only
  }
}


//DFPlayer Helper Functions: This was needed correction in checksum calculation. :)
void DFPlayerSetting(byte CMD, byte Par1, byte Par2){
  word checksum = -(0xFF + 0x06 + CMD + 0x00); //CORRECTION1: Par1 and Par2 are not included.
  byte Command_line[10] = { 0x7E, 0xFF, 0x06, CMD, 0x00,Par1, Par2, highByte(checksum), lowByte(checksum), 0xEF};
  for (byte k=0; k<10; k++)
  {
    mySerial.write( Command_line[k]);
  }
  delay(500); //make sure you wait at least half second before sending next command. otherwise next command might not be accepted.
}
//----------------------------------------------------------------
