
#include "WindowsSerial\WindowsSerial.h"
#include <stdio.h>
#include <time.h>
#include <string>
#include <iostream>

int main(void) {
    WindowsSerial test;

    int st = test.OpenPort(11);

    if(st == 0){
        printf("OPEN SERIAL PORT\n");
        printf("COM%d : %d\n", test.GetConnectCOM(), test.GetConnectState());
    }
    else {
        printf("***NO OPEN SERIAL PORT***\n");
        printf("st = %d", st);
        return 0;
    }
    

    while(1) {
        std::string str = "";
        printf(">>>");
        std::getline(std::cin, str);
        
        if(str == "end"){
            break;
        }
    }

    test.ClosePort();

    printf("CLOSE SERIAL PORT\n");
    printf("COM%d : %d\n", test.GetConnectCOM(), test.GetConnectState());

    return 0;
    
}