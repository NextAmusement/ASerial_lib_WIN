#include "ASerial_lib_Controller_Win.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string>
#include <wchar.h>
#include "WindowsSerial.h"

ASerial_lib_Controller_Win::ASerial_lib_Controller_Win(int target_device_id, int baudrate) : ASerialPacket(target_device_id){
    
}
/*
int ASerial_lib_Controller_Win::OpaenSerialPort(int com_num, int receive_buffer, int transmit_buffer, int read_interval_timeout, int read_timeout, int write_timeout){
    int ret = 0;
	
	char com_name[20];

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
	return ret;

}

int ASerial_lib_Controller_Win::ConnectDevice(int com_num) {


    return 0;
}

int ASerial_lib_Controller_Win::GetReceiveDetaByte(void) {
	COMSTAT ComStat;

	ClearCommError(m_serial_handle, NULL, &ComStat);

    return (int)ComStat.cbInQue;
}

int ASerial_lib_Controller_Win::GetConnectCOMnum(void) {
	int ret;

	if(!GetConnectFlag()){
		ret = 0;
	}
	else{
		ret = m_connect_comnum;
	}

    return ret;
}

int ASerial_lib_Controller_Win::clear_buffer(void) {
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

// int ASerial_lib_Controller_Win::Write(std::string str) {
// 	if(!GetConnectFlag()){
// 		return -1;
// 	}

// 	DWORD dwSendSize;
// 	DWORD inbyte = str.length();

// 	printf("length:%d\n", str.length());

// 	int Ret = WriteFile(    //データの送信
// 		m_serial_handle,          // 　通信デバイスのハンドル：CreateFile()で取得したハンドルを指定
// 		str.c_str(),  //　送信データのポインタを指定
// 		inbyte,                // 　送信するデータのバイト数を指定
// 		&dwSendSize, //  実際に送信されたバイト数（DWORD)が格納されるポインタを指定
// 		NULL          // 　　通信とは関係ない引数なのでNULLを指定　　　　
// 	);

// 	if (Ret == FALSE) {  //失敗した場合
// 		return -1;
// 	}

//     return (int)dwSendSize;
// }

int ASerial_lib_Controller_Win::Write(uint8_t data, bool flag) {
	if(!GetConnectFlag()){
		return -1;
	}

	DWORD dwSendSize;

	if(flag == true && (data == START_FLAG || data == ADD_FLAG)) {
		data -= 1;
		uint8_t add_flag = ADD_FLAG;

		int Ret = WriteFile(    //データの送信
			m_serial_handle,          // 　通信デバイスのハンドル：CreateFile()で取得したハンドルを指定
			&add_flag,  //　送信データのポインタを指定
			1,                // 　送信するデータのバイト数を指定
			&dwSendSize, //  実際に送信されたバイト数（DWORD)が格納されるポインタを指定
			NULL          // 　　通信とは関係ない引数なのでNULLを指定　　　　
		);

		if(dwSendSize != 1 || Ret == FALSE) {
			return -1;
		}
	}

	int Ret = WriteFile(    //データの送信
		m_serial_handle,          // 　通信デバイスのハンドル：CreateFile()で取得したハンドルを指定
		&data,  //　送信データのポインタを指定
		1,                // 　送信するデータのバイト数を指定
		&dwSendSize, //  実際に送信されたバイト数（DWORD)が格納されるポインタを指定
		NULL          // 　　通信とは関係ない引数なのでNULLを指定　　　　
	);

	if(dwSendSize != 1 || Ret == FALSE) {
		return -1;
	}


    return 0;
}

// int ASerial_lib_Controller_Win::CommandWrite(const int command) {
//     if(!GetConnectFlag()) {
// 		return -1;
// 	}

// 	char str[255];

// 	sprintf(str, "%x!%x!%x!%x/", m_device_id, 0, command, m_device_id + 0);

// 	int ret = Write(str);

//     return ret;
// }

int ASerial_lib_Controller_Win::CommandWrite(uint8_t command) {
	if(!GetConnectFlag()) {
		return -1;
	}

	//startflag write
	int ret = Write(START_FLAG, false);
	if(ret == -1) {
		return -1;
	}

	int ret = Write(m_device_id);
	if(ret == -1) {
		return -1;
	}

	int ret = Write(0);
	if(ret == -1) {
		return -1;
	}

	int ret = Write(command);
	if(ret == -1) {
		return -1;
	}

	int ret = Write(0);
	if(ret == -1) {
		return -1;
	}


    return 0;
}

// int ASerial_lib_Controller_Win::CommandWrite(const int command, const int data_num, const int *data_array)
// {
//     if(!GetConnectFlag()) {
// 		return -1;
// 	}

// 	std::string format_str;
// 	std::string format_data_str;
// 	std::string check_data_str;

// 	const int buf_size = 30;

// 	for(int i = 0; i < data_num; ++i){
// 		char buf[buf_size];

// 		sprintf(buf, "%x!", data_array[i]);

// 		format_data_str += buf;
// 	}

// 	char buf[buf_size];

// 	sprintf(buf, "%x!%x!%x!", m_device_id, data_num, command);

// 	format_str = buf;

// 	format_str += format_data_str;

// 	for(int i = 0; i < buf_size; ++i){
// 		buf[i] = '\0';
// 	}

// 	sprintf(buf, "%x/", m_device_id + data_num);

// 	check_data_str = buf;

// 	format_str += check_data_str;

// 	int ret = Write(format_str);

//     return ret;
// }

int ASerial_lib_Controller_Win::Read(void) {
	if(!GetConnectFlag()) {
		return -2;
	}

	uint8_t read_data;
	DWORD dwSendSize;

	int ret = ReadFile(   // データの受信
		m_serial_handle,   // 　通信デバイスのハンドル：　CreateFile()で取得したハンドルを指定
		&read_data,       // 受信バッファーのポインタを指定：　受信データがここに格納されます。
		1,         //　受信するバイト数を指定：　ここで指定するバイト数を受信するかまたはタイムアウト時間がくるまで
		// ReadFile()関数は（　getc()のように　）待ちます
		&dwSendSize,  //  実際に受信したバイト数（DWORD)が格納されるポインタを指定
		NULL   // 通信とは関係ない引数なのでNULLを指定
	);

	if(read_data == ADD_FLAG) {
		int ret = ReadFile(   // データの受信
			m_serial_handle,   // 　通信デバイスのハンドル：　CreateFile()で取得したハンドルを指定
			&read_data,       // 受信バッファーのポインタを指定：　受信データがここに格納されます。
			1,         //　受信するバイト数を指定：　ここで指定するバイト数を受信するかまたはタイムアウト時間がくるまで
			// ReadFile()関数は（　getc()のように　）待ちます
			&dwSendSize,  //  実際に受信したバイト数（DWORD)が格納されるポインタを指定
			NULL   // 通信とは関係ない引数なのでNULLを指定
		);

		read_data += 1;
	}

	if(ret == FALSE) {
		return -1;
	}

    return read_data;
}

int ASerial_lib_Controller_Win::ReadFomatDatas(uint8_t *data_buf, const int array_num) {
	





    return 0;
}

int ASerial_lib_Controller_Win::Available(void) {
	

    return 0;
}

*/