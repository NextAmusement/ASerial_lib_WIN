
#include "ASerial_Controller.h"
#include <stdio.h>
#include <time.h>

int main(void){
    ASerial test(1004, 1);

    test.OpaenSerialPort(7);

    printf("COM%d : %d\n", test.GetConnectCOMnum(), test.GetConnectFlag());

    std::string buf = "\0";

    // for(int i = 0; i < 100000; ++i){
    //     ;
    // }

    test.CommandWhite(12);
    test.Read(&buf);

    printf("Re :%s\n", buf.c_str());
    

    // test.White("bcdefg/");
    // test.Read(&buf);
    
    // printf("char :%d\n", buf[0]);
    // printf("Re :%s\n", buf.c_str());

    // clock_t Time = clock();

    // for(int i = 0; i < 1; ++i){
    //     // const int data[5] = {20, 50, 0xFF, 12, 45};
    //     // test.CommandWhite(12, 5, data);

        
        
    //     printf("WhiteState : %d\n", test.White("3EC!A!1F!12!A7!FF!0!0!BF!A!EN!B3!0!3F6/"));
    //     printf("ReadState : %d\n", test.Read(&buf));
    //     printf("Re :%s\n", buf.c_str());

    // }

    // printf("Time : %d\n", clock() - Time);

    test.ClosePort();

    printf("COM%d : %d\n", test.GetConnectCOMnum(), test.GetConnectFlag());

    return 0;
    
}