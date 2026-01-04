//マルチスレッドモードでの自動接続のテスト

#include "../ASerial_lib_Controller_Win_MT.h"
#include "../WindowsSerial/WindowsSerial.h"

#define TARGET_DEVICE_ID 0x01
#define TARGET_DEVICE_VER 0x01

int main(void) {
    printf("### ASerial_lib_Controller_Win_MT Auto Connect Test ###\n");

    printf("インスタンス生成\n");
    WindowsSerial Serial;

    ASerial_lib_Controller_Win_MT test(TARGET_DEVICE_ID, TARGET_DEVICE_VER);

    printf("インターフェース設定\n");

    test.SetInterfacePt(&Serial);

    printf("ログ設定\n");
    test.SetLogOutputFunc([](std::string log_str) {
        printf("%s", log_str.c_str());
    });

    test.SetDebugLogSetting(ASerial_lib_Controller_Win_MT::DEBUGLOG_ON_NORMAL);

    printf("自動接続スレッド作成\n");
    if (test.MakeAutoConnectThread() != 0) {
        printf("Auto Connect Thread Create Error...\n");
        return -1;
    }

    printf("自動接続スレッド開始\n");
    if (test.StartAutoConnect() != 0) {
        printf("Auto Connect Thread Start Error...\n");
        return -1;
    }

    // スレッドの終了を待つ
    printf("自動接続スレッド終了待ち\n");
    while (true) {
        int thread_state = test.GetAutoConnectThreadState();
        if (thread_state == ASerial_lib_Controller_Win_MT::AUTO_CONNECT_THREAD_END ||
            thread_state == ASerial_lib_Controller_Win_MT::AUTO_CONNECT_THREAD_ERROR) {
            printf("自動接続スレッド終了検出\n");
            break;
        }
        Sleep(100);
    }

    int com_num = test.GetConnectComNum();
    if (com_num == -1) {
        printf("接続できませんでした...残念...\n");
        printf("No Connection COM...\n");
    }
    else {
        printf("接続できました！！やったね！\n");
        printf("Connection COM %d\n", com_num);
        Sleep(100);
        test.DisConnectDevice();
    }

    printf("===============================\n");
    printf("スレッドの終了処理\n");
    printf("===============================\n");

    test.DeleteAutoConnectThread();

    test.DisConnectDevice();

    return 0;
}

