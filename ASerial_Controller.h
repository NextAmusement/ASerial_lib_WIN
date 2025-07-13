//include
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string>

#ifndef NEXTAMUSEMENT_NA_ASERIAL_ASERIAL_H_
#define NEXTAMUSEMENT_NA_ASERIAL_ASERIAL_H_




#define START_FLAG 0xd0
#define ADD_FLAG 0xad

class ASerial{
 public:
   //デフォルト値定義
   static const int ASERIAL_BAUDRATE_NORMAL = 115200;     //デフォルトのボーレート
   static const int ASERIAL_RECEIVEBUFFER_NORMAL = 1024;  //デフォルト受信バッファメモリサイズ[byte]
   static const int ASERIAL_TRANSMITBUFFER_NOMAL = 1024;  //デフォルト送信バッファメモリサイズ[byte]
   static const int ASERIAL_READTIMEOUT_NORMAL = 500;     //デフォルト受信タイムアウト時間[ms]
   static const int ASERIAL_WRITETIMEOUT_NORMAL = 500;    //デフォルト送信タイムアウト時間[ms]
   static const int ASERIAL_READ_INTERVAL_TIMEOUT_NORMAL = 50; //デフォルト読み込みインターバルタイム


    //  @brief  ASerialオブジェクトを生成する
    //  @param  target_device_id  ターゲットデバイスID
    //  @param  target_device_ver ターゲットデバイスVer
    //  @param  baudrate    ボーレート(デフォルト115200bps)*省略可
    ASerial(int target_device_id, int target_device_ver, int baudrate = ASERIAL_BAUDRATE_NORMAL);


    //  @brief  シリアルポートをオープンする
    //  @param  com_num オープンするポート番号
    int OpaenSerialPort(int com_num,
                        int receive_buffer = ASERIAL_RECEIVEBUFFER_NORMAL,
                        int transmit_buffer = ASERIAL_TRANSMITBUFFER_NOMAL, 
                        int read_interval_timeout = ASERIAL_READ_INTERVAL_TIMEOUT_NORMAL, 
                        int read_timeout = ASERIAL_READTIMEOUT_NORMAL, 
                        int write_timeout = ASERIAL_WRITETIMEOUT_NORMAL
                       );

    //  @brief  シリアルポートをクローズする
    //  @return 0:成功 -1:失敗
    int ClosePort(void);

    //  @brief  デバイスを接続する(IDとVerの照合)
    //  @param  com_num: 接続するCOM番号
    //  @return 0:接続成功
    //  @return -1:接続失敗
    int ConnectDevice(int com_num);

    //  @brief  受信バッファに溜まっているデータ数を取得
    //  @return データ数(byte)
    int GetReceiveDetaByte(void);

    //  @brief  接続ステータスを取得
    //  @return 0:未接続 1:接続
    BOOL GetConnectFlag(void);

    //  @brief  ターゲットデバイスIDを取得
    //  @return ターゲットデバイスID
    int GetTargetDeviceID(void);

    //  @brief  ターゲットデバイスVerを取得
    //  @return ターゲットデバイスVer
    int GetTargetDeviceVer(void);

    //  @brief  接続しているポート番号を取得
    //  @return 0:未接続 0以外:ポート番号
    int GetConnectCOMnum(void);
    
    //  @brief  通信を中止し、送受信バッファをクリアする
    //  @return 0:成功
    //  @return -1:失敗
    int clear_buffer(void);

    //  @brief  1Byteのデータを送信
    //  @param  data: 送信するデータ
    //  @return -1:送信失敗(未接続含む)
    //  @return 0:送信成功
    int Write(uint8_t data, bool flag = true);

    //  @brief    コマンドの送信(データ無しコマンド)
    //  @param    command : 送信するコマンド
    //  @return   0:正常送信 -1:送信失敗 
    int CommandWrite(uint8_t command);

    //  @brief    コマンドの送信
    //  @param    command : 送信するコマンド
    //  @param    data_num : データ数
    //  @param    *data_array:データが格納されている配列アドレス
    //  @return   0:正常送信 -1:送信失敗
    int CommandWrite(uint8_t command, const int data_num, uint8_t *data_array);

    //  @brief    データを1バイト受信
    //  @return   -1:受信タイムアウト
    //  @return   -2:未接続
    //  @return   それ以外:受信したデータ
    int Read(void);

    int ReadFomatDatas(uint8_t *data_buf, const int array_num);

    int Available(void);

 private:
    // @brief   通信の設定を行う(内部完結)
    // @param   baudrate 通信速度(ボーレート)
    int ComSetting(int baudrate);

    // @brief   送受信バッファの設定
    // @param   receive_buffer 受信のバッファーサイズ(byte)
    // @param   transmit_buffer   送信のバッファーサイズ(byte)
    int SetBuffer(int receive_buffer, int transmit_buffer);

    // @brief   タイムアウト含む時間関係の設定
    // @param   read_interval_timeout   文字読込時の二文字間の全体待ち時間(msec)
    // @param   read_timeout   読込エラー検出用のタイムアウト時間(msec)
    // @param   write_timeout  書き込みエラー検出用のタイムアウト時間(msec)
    int SetTimeout(int read_interval_timeout, int read_timeout, int write_timeout);

    //long StringtoReadl(std::string *str, const char cut_c);

   

    HANDLE m_serial_handle = nullptr;
    int m_connect_comnum = 0;  //接続しているポート番号
    int m_baudrate = 0;    //通信速度(ボーレート)
    int m_device_id = 0;
    int m_device_ver = 0;
    bool m_connect_flag = false;    //接続状況フラグ(false:未接続 true:接続中)
    bool m_connect_permission = false; //



};

#endif  //NEXTAMUSEMENT_NA_ASERIAL_ASERIAL_H_