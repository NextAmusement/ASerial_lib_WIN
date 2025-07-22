
#include <stdio.h>
#include <time.h>

#include <iostream>
#include <string>

// WindowsSerialのテスト

#include "WindowsSerial/WindowsSerial.h"

int main(void)
{
    WindowsSerial test;

    int st = test.OpenPort(6); // COM6を開く

    if (st == 0) {
        printf("===OPEN SERIAL PORT===\n");
        printf("COM%d : %d\n", test.GetConnectCOM(), test.GetState());
    }
    else {
        printf("***NO OPEN SERIAL PORT***\n");
        printf("st = %d", st);
        return 0;
    }

    while (1) {
        std::string str = "";
        printf(">>>");
        std::getline(std::cin, str);

        if (str == "end") {
            break;
        }

        test.write((uint8_t*)str.c_str(), (int)str.length());

        Sleep(3);

        while (test.available() > 0) {
            char read_c = (char)test.read();
            if (read_c == -1) {
                printf("**read miss**\n");
            }
            else {
                printf("%c", read_c);
            }
        }

        printf("\n");

        test.clear();
    }

    test.ClosePort();

    printf("===CLOSE SERIAL PORT===\n");
    printf("COM%d : %d\n", test.GetConnectCOM(), test.GetState());

    return 0;
}