#include "ASerial_Controller.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string>
#include <wchar.h>

ASerial::ASerial(int target_device_id, int target_device_ver, int baudrate){
	m_device_id = target_device_id;
	m_device_ver = target_device_ver;
    m_baudrate = baudrate;
}

int ASerial::OpaenSerialPort(int com_num, int receive_buffer, int transmit_buffer, int read_interval_timeout, int read_timeout, int write_timeout){
    int ret = 0;
	
	char com_name[20];

	//swprintf_s(com_name, 20, L"COM%d\0", com_num);

    sprintf_s(com_name, 20, "COM%d\0", com_num);

	 
    m_serial_handle = CreateFileA(
			com_name,     //　　ポートの名前： どのポートを開くのか
			GENERIC_READ | GENERIC_WRITE,    //　アクセスモード：　通常送受信ともするので読書き両方を指定
			0,  //　　共有モード：　通常0に設定　再オープン禁止
			NULL,  //セキュリティアトリビュート：通信では通常NULLに設定　
			OPEN_EXISTING,  //　クリエイトﾃﾞｨｽﾄﾘビューション：通常COMポートはすでに存在しているのでOPEN_EXISTINGとします。
			FILE_ATTRIBUTE_NORMAL,  //　属性：通信の場合属性はないのでFILE_ATTRIBUTE_NORMAL（属性なし）を指定　
			NULL    //　テンプレートのハンドル：　通信の場合関係ない　通常NULLを指定
		);

	if (m_serial_handle == INVALID_HANDLE_VALUE) {  //ハンドル取得に失敗した場合
		ret = -1;
	}

	if(ret == 0){
		int st = ComSetting(m_baudrate);
		if(st == -1){
			ret = -2;
		}
	}

	if(ret == 0){
		int st = SetBuffer(receive_buffer, transmit_buffer);
		if(st == -1){
			ret = -3;
		}
	}

	if(ret == 0){
		int st = SetTimeout(read_interval_timeout, read_timeout, write_timeout);
		if(st == -1){
			ret = -4;
		}
	}

	if(ret != 0){
		ClosePort();
		return ret;
	}

	m_connect_comnum = com_num;
	m_connect_flag = true;
	return ret;

}


int ASerial::ClosePort(void) {
	int ret = CloseHandle(m_serial_handle);
	m_connect_comnum = 0;
	m_connect_flag = false;
	if(ret == FALSE){
		return -1;
	}

    return 0;
}

int ASerial::ConnectDevice(int com_num) {


    return 0;
}

int ASerial::GetReceiveDetaByte(void) {
	COMSTAT ComStat;

	ClearCommError(m_serial_handle, NULL, &ComStat);

    return (int)ComStat.cbInQue;
}

BOOL ASerial::GetConnectFlag(void) {
    BOOL ret = 0;
	if(m_connect_flag) {
		ret = 1;
	}
	else{
		ret = 0;
	}

    return ret;
}

int ASerial::GetTargetDeviceID(void) {
    return m_device_id;
}

int ASerial::GetTargetDeviceVer(void) {
    return m_device_ver;
}

int ASerial::GetConnectCOMnum(void) {
	int ret;

	if(!GetConnectFlag()){
		ret = 0;
	}
	else{
		ret = m_connect_comnum;
	}

    return ret;
}

int ASerial::clear_buffer(void) {
	int ret = PurgeComm(   //消去
		m_serial_handle, // 　通信デバイスのハンドル：CreateFile()で取得したハンドルを指定
		PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR
		//   実行する操作： 上記は未処理の読書きの中止及び送受信のバッファーのクリアを指定
	);

	if (ret == FALSE) {  //失敗した場合
		return -1;
	}

    return 0;
}

int ASerial::White(std::string str) {
	if(!GetConnectFlag()){
		return -1;
	}

	DWORD dwSendSize;
	DWORD inbyte = strlen(str.c_str());

	int Ret = WriteFile(    //データの送信
		m_serial_handle,          // 　通信デバイスのハンドル：CreateFile()で取得したハンドルを指定
		str.c_str(),  //　送信データのポインタを指定
		inbyte,                // 　送信するデータのバイト数を指定
		&dwSendSize, //  実際に送信されたバイト数（DWORD)が格納されるポインタを指定
		NULL          // 　　通信とは関係ない引数なのでNULLを指定　　　　
	);

	if (Ret == FALSE) {  //失敗した場合
		return -1;
	}

    return (int)dwSendSize;
}

int ASerial::CommandWhite(const int command) {
	if(!GetConnectFlag()) {
		return -1;
	}

	char str[255];

	sprintf(str, "%x!%x!%x!%x/", m_device_id, 0, command, m_device_id + 0);

	int ret = White(str);

    return ret;
}

int ASerial::CommandWhite(const int command, const int data_num, const int *data_array) {
	if(!GetConnectFlag()) {
		return -1;
	}

	std::string format_str;
	std::string format_data_str;
	std::string check_data_str;

	const int buf_size = 30;

	for(int i = 0; i < data_num; ++i){
		char buf[buf_size];

		sprintf(buf, "%x!", data_array[i]);

		format_data_str += buf;
	}

	char buf[buf_size];

	sprintf(buf, "%x!%x!%x!", m_device_id, data_num, command);

	format_str = buf;

	format_str += format_data_str;

	for(int i = 0; i < buf_size; ++i){
		buf[i] = '\0';
	}

	sprintf(buf, "%x/", m_device_id + data_num);

	check_data_str = buf;

	format_str += check_data_str;

	int ret = White(format_str);

    return ret;
}

int ASerial::Read(std::string *str_buf) {
	if(!GetConnectFlag()){
		*str_buf = "\0";
		return 1;
	}

	DWORD dwSendSize;
	const int tofu_size = 2;
	std::string in_buf;
	clock_t Time = clock();

	while(clock() - Time < ASERIAL_READTIMEOUT_NORMAL){
		char in;

		int ret = ReadFile(   // データの受信
			m_serial_handle,   // 　通信デバイスのハンドル：　CreateFile()で取得したハンドルを指定
			&in,       // 受信バッファーのポインタを指定：　受信データがここに格納されます。
			1,         //　受信するバイト数を指定：　ここで指定するバイト数を受信するかまたはタイムアウト時間がくるまで
			// ReadFile()関数は（　getc()のように　）待ちます
			&dwSendSize,  //  実際に受信したバイト数（DWORD)が格納されるポインタを指定
			NULL   // 通信とは関係ない引数なのでNULLを指定
		);

		if(ret == FALSE){
			*str_buf = "\0";
			return -2;
		}
		
		in_buf += in;

		printf("in:%c\n", in);

		if(in == '/'){
			break;
		}
	}

	if(clock() - Time >= ASERIAL_READTIMEOUT_NORMAL){
		*str_buf = "\0";
		return -1;
	}

	//tofu文字対策

	int iti = in_buf.find((char)-16);

	if(iti != std::string::npos) {
		in_buf = in_buf.substr(iti + tofu_size);
	}

	//バッファへのコピー
	*str_buf = in_buf;

    return (int)dwSendSize;
}

int ASerial::ReadFomatDatas(long *data_buf, const int array_num) {
	if(!GetConnectFlag()) {
		return -1;
	}
	
	std::string read_str;



    return 0;
}

int ASerial::ComSetting(int baudrate){
    DCB dcb;

	//通信の設定
	GetCommState(m_serial_handle, &dcb);

	dcb.DCBlength = sizeof(DCB);        //　 DCBのサイズ：

	//データ
	dcb.BaudRate = baudrate;                //  伝送速度：　ボーレートをbps単位で指定
	dcb.fBinary = TRUE;                 //    バイナリモード：  通常TRUEに設定
	dcb.ByteSize = 8;                    //　   データサイズ　：　通常　8 ビット
	dcb.fParity = NOPARITY;         //  パリティ有無種類　：　パリティなしの場合はNOPARITYを指定
	//                                  奇数パリティの場合は ODDPARITY 　他
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

	int Ret = SetCommState(m_serial_handle, &dcb);  //SetCommState()関数にポートハンドルおよびdcb構造体のアドレスを代入する

    if(Ret == FALSE) {
        return -1;
    }

    return 0;
}

int ASerial::SetBuffer(int receive_buffer, int transmit_buffer) {
	int Ret = SetupComm(       //設定
		m_serial_handle, // 　通信デバイスのハンドル：CreateFile()で取得したハンドルを指定
		receive_buffer,   //   受信バッファーサイズ：　受信のバッファーサイズをバイト単位で指定
		transmit_buffer    // 　送信バッファーサイズ：　送信のバッファーサイズをバイト単位で指定
	);


	if (Ret == FALSE) {  //　失敗した場合
		return -1;
	}

    return 0;
}

int ASerial::SetTimeout(int read_interval_timeout, int read_timeout, int write_timeout) {
    COMMTIMEOUTS timeout;

    timeout.ReadIntervalTimeout = read_interval_timeout; // 文字読込時の二文字間の全体待ち時間（msec）

	timeout.ReadTotalTimeoutMultiplier = 0; //読込の１文字あたりの時間
	timeout.ReadTotalTimeoutConstant = read_timeout; //読込エラー検出用のタイムアウト時間
	//(受信トータルタイムアウト) = ReadTotalTimeoutMultiplier * (受信予定バイト数) + ReadTotalTimeoutConstant

	timeout.WriteTotalTimeoutMultiplier = 0; //書き込み１文字あたりの待ち時間
	timeout.WriteTotalTimeoutConstant = write_timeout;//書き込みエラー検出用のタイムアウト時間
	//(送信トータルタイムアウト) = WriteTotalTimeoutMultiplier * (送信予定バイト数) + WriteTotalTimeoutConstant

	int Ret = SetCommTimeouts(m_serial_handle, &timeout);//SetCommTimeOut()関数にポートハンドルおよびCOMMTIMEOUTS構造体の
	//アドレスを代入します。

	if (Ret == FALSE) { //失敗した場合
		return -1;
	}

    return 0;
}

long ASerial::StringtoReadl(std::string *str, const char cut_c) {
	int iti = 0;

	iti = str->find(cut_c);

	if(iti == std::string::npos){
		return -1;
	}

	std::string num_str = str->substr(0, iti);

	str->erase(0, iti + 1);

	//const char* num = num_str.c_str();

	return strtol(num_str.c_str(), NULL, 16);
}
