// テストプログラム(自転車デバイスのRPM取得)
// このプログラムは、ASerial_lib_Controller_Winを使用して
// 自転車デバイスからRPMを取得するためのものです。

#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "../ASerial_lib_Controller_Win.h"

namespace MyConst {
constexpr uint8_t DEVICE_ID = 0x03;           // デバイスID
constexpr uint8_t DEVICE_VER = 0x01;          // デバイスバージョン
constexpr uint8_t GET_UPDATE_COMMAND = 0x20;  // 更新フラグ取得コマンド
constexpr uint8_t GET_RPM_COMMAND = 0x21;     // RPM取得コマンド
}  // namespace MyConst

namespace MyObj {
ASerial_lib_Controller_Win aserial(MyConst::DEVICE_ID, MyConst::DEVICE_VER);
WindowsSerial serial;
}  // namespace MyObj

int main(void)
{
    // setup
    MyObj::aserial.SetInterfacePt(&MyObj::serial);
    int st = MyObj::aserial.AutoConnectDevice();  // 自動接続

    if (st == -1) {  // 接続失敗
        printf("[ERROR]NoConnectDevice");
        return 0;
    }
    else {  // 接続成功
        printf("ConnectDevice COM %d\n", st);
    }

    int count = 0;  // RPMの取得回数
    printf("GetRPMcount : ");
    scanf("%d", &count);

    clock_t fps_time = 0;
    const int fps = 120;  // フレームレート
    // フレームレートに基づいてフレーム間の遅延
    const int fps_delay = (1.0f / fps) * 1000;

    int get_data_count = 0;
    int update_command_flag = 0;

    // メインループ
    while (get_data_count < count) {
        fps_time = clock();  // フレーム時間の初期化

        ASerialDataStruct::ASerialData read_data;  // 読み取りデータバッファ

        // 更新フラグの取得とRPMの取得
        if (update_command_flag == 0) {                             // 更新フラグ取得
            MyObj::aserial.WriteData(MyConst::GET_UPDATE_COMMAND);  // 更新フラグ取得コマンド送信
            int st = MyObj::aserial.ReadData(&read_data);           // 更新フラグ読み取り
            if (st == 0 && read_data.data[0] == 1) {                // 更新フラグが1かつ正常読み取りの場合
                update_command_flag = 1;
            }
            else if (st == -1) {  // 読み取りエラー
                printf("[ERROR]ReadError\n");
                // エラーが発生した場合は、再度同じ通信を行う。
            }
            else if (st == -2) {  // タイムアウトエラー
                printf("[ERROR]ReadTimeout\n");
                // もしタイムアウトや読み取りエラーが発生した場合は、再度同じ通信を行う。
            }
        }
        else { // RPMの取得
            MyObj::aserial.WriteData(MyConst::GET_RPM_COMMAND); // RPM取得コマンド送信
            int st = MyObj::aserial.ReadData(&read_data); // RPM読み取り
            if (st == 0) {  // RPMの正常読み取り
                uint16_t rpm = (((uint16_t)read_data.data[0] << 8) | (uint16_t)read_data.data[1]);
                printf("Count:\t%d\tRPM:\t%d\n", get_data_count, rpm);
                update_command_flag = 0;
                ++get_data_count;
            }
            else if (st == -1) {  // 読み取りエラー
                printf("[ERROR]ReadError\n");
                // エラーが発生した場合は、再度同じ通信を行う。
            }
            else if (st == -2) {  // タイムアウトエラー
                printf("[ERROR]ReadTimeout\n");
                // もしタイムアウトや読み取りエラーが発生した場合は、再度同じ通信を行う。
            }
        }

        // フレームレートに基づいて遅延
        while (clock() - fps_time < fps_delay) {
            ;
        }
    }

    //終了処理
    MyObj::aserial.DisConnectDevice();// デバイスの切断
    return 0;// プログラム終了
}