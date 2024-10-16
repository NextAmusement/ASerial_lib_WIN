#include "ASerial_ArduinoSide.h"


String function;

int ST = 0;
int argument[10];
int ASerial_Readfuncion(String* function, int* argument, int arrayNum, int Timeout = 1000);


void setup() {
  Serial.begin(250000);
}

void loop() {

  if (Serial.available() > 0) {
    ST = ASerial_Readfuncion(&function, argument, 10, 1000);
    Serial.println(ST);
    if (ST == 0) {
      Serial.print("function:");
      Serial.println(function);
      Serial.flush();
    }
  } else {
    function = "";
    for (int i = 0; i < 10; i++) {
      argument[i] = 0;
    }
  }
}


