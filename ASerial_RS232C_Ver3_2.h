/*==============================================================*/
//2024(C)NextAmusement  Ver3.2
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

==========================================================================
わざわざ英語にする必要ないな。どうせ自分しか使わないし。
気が向いたときに全てクラス化するか…
スパゲッティになりかけてて怖い。
極論、動けば良いんだけどさ。
無駄が多すぎる。まあ、まだCをかじりかけの時に作り始めたし仕方ないか。
うん。見にくい。
変更履歴に日付書いてないのなんで？(過去の自分へ)

V3_2
==========================================================================
更新日:2024/07/07

*/

/*revision history*/
/*Ver1.0  completion*/
/*Ver1.5  Changed from sending data of type int as it is to sending it after converting it to a string.*/
/*Ver2.0  Overall specification changed to HANDLE specification type.*/
/*Ver3.1  Readint関数をタイムアウトまたはデータ受信するまで待機するように仕様変更*/
/*Ver3.2  ReadintArray関数を追加----24/07/07															*/
/*==============================================================*/

#pragma once

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <time.h>




#define ASERIAL_BAUDRATE_NORMAL 9600
#define ASERIAL_RECEIVEBUFFER_NORMAL 1024
#define ASERIAL_TRANSMITBUFFER_NOMAL 1024
#define ASERIAL_READTIMEOUT_NORMAL 1000
#define ASERIAL_WRITETIMEOUT_NORMAL 1000


#define DEC 10
#define BIN 2
#define HEX 16




/*=========================================================*/
/*=================prototype declaration===================*/

extern int ASerial_Open(unsigned int, HANDLE*, int = ASERIAL_BAUDRATE_NORMAL, int = ASERIAL_RECEIVEBUFFER_NORMAL, int = ASERIAL_TRANSMITBUFFER_NOMAL, int = ASERIAL_READTIMEOUT_NORMAL, int = ASERIAL_WRITETIMEOUT_NORMAL);
extern int ASerial_Clearbuffer(HANDLE);
extern int ASerial_Setbuffer(HANDLE, int, int);
extern int ASerial_Settimeout(HANDLE, int, int);
extern int ASerial_Writeint(HANDLE, int);
extern int ASerial_Write(HANDLE, char*);
extern int ASerial_Read(HANDLE, char*);
extern int ASerial_Readint(HANDLE, int);
extern int ASerial_Readintarray(HANDLE, int* inarraybuf, int Elementnum, int base = DEC, const char cutchar = '!', const char endchar = '/');
extern int ASerial_Close(HANDLE);

/*===========================================================*/
//Open serial ports COM1-99(baudrate:9600)
inline int ASerial_Open(unsigned int COMnum, HANDLE* handlein, int baudrate, int receivebuffer, int transmitbuffer, int readTimeOut, int writeTimeOut) {

	HANDLE hPort = 0;

	if (COMnum <= 0 || COMnum >= 100) {
		return -5;
	}

	int dig[2];
	int num_dig = 0;
	for (int i = COMnum; i > 0; i = i / 10) {
		++num_dig;
	}

	if (num_dig == 2) {
		COMnum = COMnum % 100;
		dig[1] = COMnum / 10;
		COMnum = COMnum % 10;
		dig[0] = COMnum;

		char dig1 = '0' + dig[1];
		char dig2 = '0' + dig[0];

		char COMname[] = { 'C','O','M',dig1, dig2, '\0' };

		hPort = CreateFile(
			COMname,     //　　ポートの名前： どのポートを開くのか
			GENERIC_READ | GENERIC_WRITE,    //　アクセスモード：　通常送受信ともするので読書き両方を指定
			0,  //　　共有モード：　通常0に設定　再オープン禁止
			NULL,  //セキュリティアトリビュート：通信では通常NULLに設定　
			OPEN_EXISTING,  //　クリエイトﾃﾞｨｽﾄﾘビューション：通常COMポートはすでに存在しているのでOPEN_EXISTINGとします。
			FILE_ATTRIBUTE_NORMAL,  //　属性：通信の場合属性はないのでFILE_ATTRIBUTE_NORMAL（属性なし）を指定　
			NULL    //　テンプレートのハンドル：　通信の場合関係ない　通常NULLを指定
		);
	}
	else if (num_dig == 1) {
		COMnum = COMnum % 10;
		dig[0] = COMnum;

		char dig1 = '0' + dig[0];

		char COMname[] = { 'C','O','M',dig1,'\0' };

		hPort = CreateFile(
			COMname,     //　　ポートの名前： どのポートを開くのか
			GENERIC_READ | GENERIC_WRITE,    //　アクセスモード：　通常送受信ともするので読書き両方を指定
			0,  //　　共有モード：　通常0に設定　再オープン禁止
			NULL,  //セキュリティアトリビュート：通信では通常NULLに設定　
			OPEN_EXISTING,  //　クリエイトﾃﾞｨｽﾄﾘビューション：通常COMポートはすでに存在しているのでOPEN_EXISTINGとします。
			FILE_ATTRIBUTE_NORMAL,  //　属性：通信の場合属性はないのでFILE_ATTRIBUTE_NORMAL（属性なし）を指定　
			NULL    //　テンプレートのハンドル：　通信の場合関係ない　通常NULLを指定
		);
	}

	if (hPort == INVALID_HANDLE_VALUE)  //ハンドル取得に失敗した場合
	{
		CloseHandle(hPort);
		return -1;
	}


	DCB dcb;


	//通信の設定
	GetCommState(hPort, &dcb);

	dcb.DCBlength = sizeof(DCB);        //　 DCBのサイズ：
	//データ
	dcb.BaudRate = baudrate;                //  伝送速度：　ボーレートをbps単位で指定
	dcb.fBinary = TRUE;                 //    バイナリモード：  通常TRUEに設定
	dcb.ByteSize = 8;                    //　   データサイズ　：　通常　8 ビット
	dcb.fParity = NOPARITY;         //  パリティ有無種類　：　パリティなしの場合はNOPARITYを指定
	//                                     奇数パリティの場合は ODDPARITY 　他
	dcb.StopBits = ONESTOPBIT;         // ストップビットの数：　通常１ビット→ ONESTOPBIT;


	//ハードウェアフロー制御
	dcb.fOutxCtsFlow = FALSE;       // 　CTSハードウェアフロー制御：CTS制御を使用しない場合はFLASEを指定
	//   　　　　　　CTS 制御をする場合はTRUEを指定してCTS信号を監視します。                    
	dcb.fOutxDsrFlow = FALSE;       //  DSRハードウェアフロー制御：使用しない場合はFALSEを指定
	dcb.fDtrControl = DTR_CONTROL_DISABLE;  // DTR有効/無効：　無効なら　DTR_CONTROL_DISABLE;ISABLE
	dcb.fRtsControl = RTS_CONTROL_DISABLE;  // RTS制御：　RTS制御をしない場合はRTS_CONTROL_DISABLEを指定
	//　　　　　　　 RTS制御をする場合はRTS_CONTROL_ENABLE　等を指定
// ソフトウェアフロー制御
	dcb.fOutX = FALSE;                 // 送信時XON/OFF制御の有無：　なし→FLALSE 
	dcb.fInX = FALSE;                   // 受信時XON/XOFF制御の有無：なし→FALSE 
	dcb.fTXContinueOnXoff = TRUE;    // 受信バッファー満杯＆XOFF受信後の継続送信可否：送信可→TRUE
	dcb.XonLim = 512;                 // XONが送られるまでに格納できる最小バイト数：
	dcb.XoffLim = 512;                 // XOFFが送られるまでに格納できる最小バイト数：
	dcb.XonChar = 0x11;                 // 送信時XON文字 ( 送信可：ビジィ解除 ) の指定：
	//　一般に、XON文字として11H ( デバイス制御１：DC1 )がよく使われます
	dcb.XoffChar = 0x13;             // XOFF文字（送信不可：ビジー通告）の指定：なし→FALSE
	//　一般に、XOFF文字として13H ( デバイス制御3：DC3 )がよく使われます


//その他

	dcb.fNull = TRUE;                 // NULLバイトの破棄： 破棄する→TRUE
	dcb.fAbortOnError = TRUE;         // エラー時の読み書き操作終了：　終了する→TRUE
	dcb.fErrorChar = FALSE;             // パリティエラー発生時のキャラクタ（ErrorChar）置換：　なし→FLALSE
	dcb.ErrorChar = 0x00;             // パリティエラー発生時の置換キャラクタ
	dcb.EofChar = 0x03;             // データ終了通知キャラクタ　：　一般に0x03(ETX)がよく使われます。
	dcb.EvtChar = 0x02;             // イベント通知キャラクタ ：　一般に0x02(STX)がよく使われます

	int Ret = 0;


	Ret = SetCommState(hPort, &dcb);  //SetCommState()関数にポートハンドルおよびdcb構造体のアドレスを代入する

	
	if (Ret == FALSE)  // 失敗した場合
	{
		//CloseHandle(hPort);
		return -2;
	}



	Ret = SetupComm(       //設定
		hPort, // 　通信デバイスのハンドル：CreateFile()で取得したハンドルを指定
		receivebuffer,   //   受信バッファーサイズ：　受信のバッファーサイズをバイト単位で指定
		transmitbuffer    // 　送信バッファーサイズ：　送信のバッファーサイズをバイト単位で指定
	);
	if (Ret == FALSE)  //　失敗した場合
	{
		//CloseHandle(hPort);
		return -3;
	}

	COMMTIMEOUTS timeout;

	timeout.ReadIntervalTimeout = 50; // 文字読込時の２も時間の全体待ち時間（msec）

	timeout.ReadTotalTimeoutMultiplier = 0; //読込の１文字あたりの時間
	timeout.ReadTotalTimeoutConstant = readTimeOut; //読込エラー検出用のタイムアウト時間
	//(受信トータルタイムアウト) = ReadTotalTimeoutMultiplier * (受信予定バイト数) + ReadTotalTimeoutConstant

	timeout.WriteTotalTimeoutMultiplier = 0; //書き込み１文字あたりの待ち時間
	timeout.WriteTotalTimeoutConstant = writeTimeOut;//書き込みエラー検出用のタイムアウト時間
	//(送信トータルタイムアウト) = WriteTotalTimeoutMultiplier * (送信予定バイト数) + WriteTotalTimeoutConstant

	Ret = SetCommTimeouts(hPort, &timeout);//SetCommTimeOut()関数にポートハンドルおよびCOMMTIMEOUTS構造体の
	//アドレスを代入します。

	if (Ret == FALSE) //失敗した場合
	{
		//CloseHandle(hPort);
		return -4;
	}



	*handlein = hPort;
	return 0;
}



//送受信バッファのクリア処理（-1が帰ってきたら失敗）
inline int ASerial_Clearbuffer(HANDLE hPort) {
	int Ret = 0;

	Ret = PurgeComm(   //消去
		hPort, // 　通信デバイスのハンドル：CreateFile()で取得したハンドルを指定
		PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR
		//   実行する操作： 上記は未処理の読書きの中止及び送受信のバッファーのクリアを指定
	);

	if (Ret == FALSE)  //失敗した場合
	{
		//CloseHandle(hPort);
		return -1;
	}
	return 0;
}

//送受信バッファの確保（デフォルトは1024バイト）
inline int ASerial_Setbuffer(HANDLE hPort, int receivebuffer = ASERIAL_RECEIVEBUFFER_NORMAL, int transmitbuffer = ASERIAL_TRANSMITBUFFER_NOMAL) {
	int Ret = 0;

	Ret = SetupComm(       //設定
		hPort, // 　通信デバイスのハンドル：CreateFile()で取得したハンドルを指定
		receivebuffer,   //   受信バッファーサイズ：　受信のバッファーサイズをバイト単位で指定
		transmitbuffer    // 　送信バッファーサイズ：　送信のバッファーサイズをバイト単位で指定
	);
	if (Ret == FALSE)  //　失敗した場合
	{
		//CloseHandle(hPort);
		return -1;
	}
	return 0;
}

//送受信のタイムアウト設定（デフォルトはどちらも500msec)
inline int ASerial_Settimeout(HANDLE hPort, int readTimeOut = ASERIAL_READTIMEOUT_NORMAL, int writeTimeOut = ASERIAL_WRITETIMEOUT_NORMAL) {
	COMMTIMEOUTS timeout;

	timeout.ReadIntervalTimeout = 500; // 文字読込時の２も時間の全体待ち時間（msec）

	timeout.ReadTotalTimeoutMultiplier = 0; //読込の１文字あたりの時間
	timeout.ReadTotalTimeoutConstant = readTimeOut; //読込エラー検出用のタイムアウト時間
	//(受信トータルタイムアウト) = ReadTotalTimeoutMultiplier * (受信予定バイト数) + ReadTotalTimeoutConstant

	timeout.WriteTotalTimeoutMultiplier = 0; //書き込み１文字あたりの待ち時間
	timeout.WriteTotalTimeoutConstant = writeTimeOut;//書き込みエラー検出用のタイムアウト時間
	//(送信トータルタイムアウト) = WriteTotalTimeoutMultiplier * (送信予定バイト数) + WriteTotalTimeoutConstant

	int Ret = 0;

	Ret = SetCommTimeouts(hPort, &timeout);//SetCommTimeOut()関数にポートハンドルおよびCOMMTIMEOUTS構造体の
	//アドレスを代入します。

	if (Ret == FALSE) //失敗した場合
	{
		//CloseHandle(hPort);
		return -1;
	}


	return 0;
}

//int型のデータを送信(送信自体はchar型)
inline int ASerial_Writeint(HANDLE hPort, int data) {
	DWORD dwSendSize;

	int num_dig = 0;
	for (int i = data; i > 0; i = i / 10) {
		++num_dig;
	}

	char intdata[64];

	num_dig = num_dig + 1;

	sprintf_s(intdata, num_dig, "%d", data);

	DWORD inbyte = strlen(intdata);

	int Ret = 0;

	Ret = WriteFile(    //データの送信
		hPort,          // 　通信デバイスのハンドル：CreateFile()で取得したハンドルを指定
		&intdata,  //　送信データのポインタを指定
		inbyte,                // 　送信するデータのバイト数を指定
		&dwSendSize, //  実際に送信されたバイト数（DWORD)が格納されるポインタを指定
		NULL          // 　　通信とは関係ない引数なのでNULLを指定　　　　
	);

	if (Ret == FALSE)  //失敗した場合
	{
		//CloseHandle(hPort);
		return -1;
	}
	return (int)dwSendSize;
}

//char型のデータを送信
inline int ASerial_Write(HANDLE hPort, const char* inbuf) {
	DWORD dwSendSize;
	DWORD inbyte = strlen(inbuf);

	int Ret = 0;

	Ret = WriteFile(    //データの送信
		hPort,          // 　通信デバイスのハンドル：CreateFile()で取得したハンドルを指定
		inbuf,  //　送信データのポインタを指定
		inbyte,                // 　送信するデータのバイト数を指定
		&dwSendSize, //  実際に送信されたバイト数（DWORD)が格納されるポインタを指定
		NULL          // 　　通信とは関係ない引数なのでNULLを指定　　　　
	);

	if (Ret == FALSE)  //失敗した場合
	{
		//CloseHandle(hPort);
		return -1;
	}
	return (int)dwSendSize;
}

//int型のデータを受信
inline int ASerial_Readint(HANDLE hPort, int base) {
	DWORD dwSendSize;
	COMSTAT ComStat;
	clock_t Time = clock();

	ClearCommError(hPort, NULL, &ComStat);
	DWORD Readsize = ComStat.cbInQue;

	while (Readsize <= 0 && clock() - Time < ASERIAL_READTIMEOUT_NORMAL) {
		ClearCommError(hPort, NULL, &ComStat);
		Readsize = ComStat.cbInQue;
	}




	char datain[64] = {0};
	int data = 0;

	int Ret = 0;

	if (Readsize > 0) {
		Ret = ReadFile(   // データの受信
			hPort,   // 　通信デバイスのハンドル：　CreateFile()で取得したハンドルを指定
			&datain,       // 受信バッファーのポインタを指定：　受信データがここに格納されます。
			Readsize,         //　受信するバイト数を指定：　ここで指定するバイト数を受信するかまたはタイムアウト時間がくるまで
			// ReadFile()関数は（　getc()のように　）待ちます
			&dwSendSize,  //  実際に受信したバイト数（DWORD)が格納されるポインタを指定
			NULL   // 通信とは関係ない引数なのでNULLを指定
		);

		if (Ret == FALSE)     //失敗した場合
		{

			//CloseHandle(hPort);
			return -1;
		}

		

		data = strtol(datain, NULL, base);

		return data;
	}
	else {
		return -2;
	}


	return 0;

}



inline int ASerial_Read(HANDLE hPort, char* outbuf) {

	int Ret = 0;

	DWORD dwSendSize;
	Ret = ReadFile(   // データの受信
		hPort,   // 　通信デバイスのハンドル：　CreateFile()で取得したハンドルを指定
		&outbuf,       // 受信バッファーのポインタを指定：　受信データがここに格納されます。
		64,         //　受信するバイト数を指定：　ここで指定するバイト数を受信するかまたはタイムアウト時間がくるまで
		// ReadFile()関数は（　getc()のように　）待ちます
		&dwSendSize,  //  実際に受信したバイト数（DWORD)が格納されるポインタを指定
		NULL   // 通信とは関係ない引数なのでNULLを指定
	);

	if (Ret == FALSE)     //失敗した場合
	{

		//CloseHandle(hPort);
		return -1;
	}


	return 0;
}

//int型のデータ群の読み込み
int ASerial_Readintarray(HANDLE hPort, int* inarraybuf, int Elementnum, int base, const char cutchar, const char endchar){
	std::string inbuf;
	int Ret = 0;


	DWORD dwSendSize;
	COMSTAT ComStat;
	clock_t Time = clock();

	ClearCommError(hPort, NULL, &ComStat);
	DWORD Readsize = ComStat.cbInQue;

	while (Readsize <= 0 && clock() - Time < ASERIAL_READTIMEOUT_NORMAL) {
		ClearCommError(hPort, NULL, &ComStat);
		Readsize = ComStat.cbInQue;
	}


	if (Readsize > 0) {
		Ret = ReadFile(   // データの受信
			hPort,   // 　通信デバイスのハンドル：　CreateFile()で取得したハンドルを指定
			&inbuf,       // 受信バッファーのポインタを指定：　受信データがここに格納されます。
			Readsize,         //　受信するバイト数を指定：　ここで指定するバイト数を受信するかまたはタイムアウト時間がくるまで
			// ReadFile()関数は（　getc()のように　）待ちます
			&dwSendSize,  //  実際に受信したバイト数（DWORD)が格納されるポインタを指定
			NULL   // 通信とは関係ない引数なのでNULLを指定
		);

		if (Ret == FALSE)     //失敗した場合
		{
			return -1;
		}

		for (int i = 0; i < Elementnum; ++i) {
			std::string numstr;
			int iti = 0;

			iti = inbuf.find(cutchar);

			if (iti == std::string::npos) {
				if (inbuf.empty())break;
				iti = inbuf.find(endchar);
				numstr = inbuf.substr(0, iti + 1);

				inarraybuf[i] = strtol(numstr.c_str(), NULL, base);
				break;

			}


			iti = inbuf.find(cutchar);
			numstr = inbuf.substr(0, iti + 1);
			inbuf.erase(0, iti + 1);

			inarraybuf[i] = strtol(numstr.c_str(), NULL, base);

		}



	}
	else {
		return -2;
	}



	return 0;
}



//ポートのクローズ
inline int ASerial_Close(HANDLE hPort) {
	CloseHandle(hPort);
	return 0;
}






