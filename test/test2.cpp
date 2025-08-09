

// 自動接続テスト

#include <stdio.h>

#include "../ASerial_lib_Controller_Win.h"
#include "../WindowsSerial/WindowsSerial.h"

// ASerial_lib_Controller_Win test(0x01, 0x01);
WindowsSerial Serial;

int main(void)
{   
    printf("Target Device ID :");
    int device_id = 0;
    scanf("%X", &device_id);

    printf("Terget Device Ver :");
    int device_ver = 0;
    scanf("%X", &device_ver);

    ASerial_lib_Controller_Win *test = new ASerial_lib_Controller_Win(device_id, device_ver);

    test->SetInterfacePt(&Serial);

    int st = test->AutoConnectDevice();

    if (st == -1) {
        printf("No Connection COM...");
    }
    else {
        printf("Connection COM %d", st);

        Sleep(100);
        test->DisConnectDevice();
    }

    delete test;

    return 0;
}