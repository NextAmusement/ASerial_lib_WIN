#include <stdio.h>

#include "ASerial_lib_Controller_Win.h"
#include "WindowsSerial/WindowsSerial.h"

ASerial_lib_Controller_Win test(0x01, 0x01);
WindowsSerial Serial;


int main(void) {
    test.SetInterfacePt(&Serial);

     int st = test.AutoConnectDevice();

     if(st == -1) {
        printf("No Connection COM...");
     }
     else {
        printf("Connection COM %d", st);

        Sleep(100);
        test.DisConnectDevice();
     }

     return 0;
}