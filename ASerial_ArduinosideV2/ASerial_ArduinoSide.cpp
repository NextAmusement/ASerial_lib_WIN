#include "HardwareSerial.h"
#include "ASerial_ArduinoSide.h"



/*シリアル通信関数読み込み関数*/
int ASerial_Readfuncion(String* function, int* argument, int arrayNum, int Timeout) {
  char tag;
  String buf;
  int i = 0;
  unsigned long Time = millis();


  Serial.setTimeout(Timeout);
  buf = Serial.readStringUntil('/');
  buf.trim();

  int ichi = buf.indexOf('!');
  if (ichi == -1) {
    *function = buf;
    return 0;
  } 
  else {
    *function = buf.substring(0, ichi);
  }

  buf = buf.substring(ichi + 1);

  


  
  while (1) {
    String num;

    int ichi = buf.indexOf('!');
    if (ichi == -1) {
      num = buf;
      argument[i] = num.toInt();
      return 0;
    } 
    else {
      num = buf.substring(0, ichi);
      argument[i] = num.toInt();
    }

    buf = buf.substring(ichi + 1);
    i++;

  }





  return -1;
}