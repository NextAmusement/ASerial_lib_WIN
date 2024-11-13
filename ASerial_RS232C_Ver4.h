/*==============================================================*/
/*2024(C)NextAmusement  Ver4.0*/
/*
This library was created by NextAmusement to allow easy data exchange
with Arduino and other microcomputers via serial communication (RS232C).
It was also created for arcade game development and may be difficult to
use for other applications.


An aside. Actually, this library just taps the Windows API directly.
However, since Ver3_2(beta) or later, we have added a function to read a group of data, so it makes sense to use this library. 
Even before that, the library had some significance because it was easy to send and receive data.
Incidentally, the intended use of the library is to make it into a class. Perhaps a sample class could be included in the package.
Or perhaps the library itself should have been made into a class...

[Dev] cha_maru

*/

/*revision history*/
/*Ver1.0  completion*/
/*Ver1.5  Changed from sending data of type int as it is to sending it after converting it to a string.*/
/*Ver2.0  Overall specification changed to HANDLE specification type.*/
/*Ver3.1  Readint関数をタイムアウトまたはデータ受信するまで待機するように仕様変更*/
/*Ver3.2  ReadintArray関数を追加----24/07/07									                        */
/*Ver4.0  ライブラリのクラス化・動作の最適化、改善(クラス化に伴いファイルの細分化)----24/09/24                                                               */
/*==============================================================*/
/*include*/
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string>


/*デフォルト値定義*/
#define ASERIAL_BAUDRATE_NORMAL 9600        //デフォルトボーレート[bps]
#define ASERIAL_RECEIVEBUFFER_NORMAL 1024   //デフォルト受信バッファメモリサイズ[byte]
#define ASERIAL_TRANSMITBUFFER_NOMAL 1024   //デフォルト送信バッファメモリサイズ[byte]
#define ASERIAL_READTIMEOUT_NORMAL 1000     //デフォルト受信タイムアウト時間[ms]
#define ASERIAL_WRITETIMEOUT_NORMAL 1000    //デフォルト送信タイムアウト時間[ms]

/*進数指定定義*/
#define DEC 10
#define BIN 2
#define HEX 16


class ASerial{
public:

    //     @brief  ASerialオブジェクトを生成する
    //     @param  baudrate    ボーレート(デフォルト9600bps)*省略可
    explicit ASerial(uint32_t = ASERIAL_BAUDRATE_NORMAL);

    ~ASerial(void);


    //  @brief  シリアルポートをオープンする
    //  @param  オープンするポート番号
    void OpaenSerialPort(uint32_t);
    


private:
    HANDLE m_serial_handle = nullptr;
    unsigned int m_conectCOMnum = 0;  //接続しているポート番号
    unsigned int m_baudrate = ASERIAL_BAUDRATE_NORMAL;    //通信速度(ボーレート)
    bool m_conect_flag = false;    //接続状況フラグ(false:未接続 true:接続中)






};
