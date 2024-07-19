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
�킴�킴�p��ɂ���K�v�Ȃ��ȁB�ǂ������������g��Ȃ����B
�C���������Ƃ��ɑS�ăN���X�����邩�c
�X�p�Q�b�e�B�ɂȂ肩���Ăĕ|���B
�ɘ_�A�����Ηǂ��񂾂��ǂ��B
���ʂ���������B�܂��A�܂�C�������肩���̎��ɍ��n�߂����d���Ȃ����B
����B���ɂ����B
�ύX�����ɓ��t�����ĂȂ��̂Ȃ�ŁH(�ߋ��̎�����)

V3_2
==========================================================================
�X�V��:2024/07/07

*/

/*revision history*/
/*Ver1.0  completion*/
/*Ver1.5  Changed from sending data of type int as it is to sending it after converting it to a string.*/
/*Ver2.0  Overall specification changed to HANDLE specification type.*/
/*Ver3.1  Readint�֐����^�C���A�E�g�܂��̓f�[�^��M����܂őҋ@����悤�Ɏd�l�ύX*/
/*Ver3.2  ReadintArray�֐���ǉ�----24/07/07															*/
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
			COMname,     //�@�@�|�[�g�̖��O�F �ǂ̃|�[�g���J���̂�
			GENERIC_READ | GENERIC_WRITE,    //�@�A�N�Z�X���[�h�F�@�ʏ푗��M�Ƃ�����̂œǏ����������w��
			0,  //�@�@���L���[�h�F�@�ʏ�0�ɐݒ�@�ăI�[�v���֎~
			NULL,  //�Z�L�����e�B�A�g���r���[�g�F�ʐM�ł͒ʏ�NULL�ɐݒ�@
			OPEN_EXISTING,  //�@�N���G�C�g�ި��؃r���[�V�����F�ʏ�COM�|�[�g�͂��łɑ��݂��Ă���̂�OPEN_EXISTING�Ƃ��܂��B
			FILE_ATTRIBUTE_NORMAL,  //�@�����F�ʐM�̏ꍇ�����͂Ȃ��̂�FILE_ATTRIBUTE_NORMAL�i�����Ȃ��j���w��@
			NULL    //�@�e���v���[�g�̃n���h���F�@�ʐM�̏ꍇ�֌W�Ȃ��@�ʏ�NULL���w��
		);
	}
	else if (num_dig == 1) {
		COMnum = COMnum % 10;
		dig[0] = COMnum;

		char dig1 = '0' + dig[0];

		char COMname[] = { 'C','O','M',dig1,'\0' };

		hPort = CreateFile(
			COMname,     //�@�@�|�[�g�̖��O�F �ǂ̃|�[�g���J���̂�
			GENERIC_READ | GENERIC_WRITE,    //�@�A�N�Z�X���[�h�F�@�ʏ푗��M�Ƃ�����̂œǏ����������w��
			0,  //�@�@���L���[�h�F�@�ʏ�0�ɐݒ�@�ăI�[�v���֎~
			NULL,  //�Z�L�����e�B�A�g���r���[�g�F�ʐM�ł͒ʏ�NULL�ɐݒ�@
			OPEN_EXISTING,  //�@�N���G�C�g�ި��؃r���[�V�����F�ʏ�COM�|�[�g�͂��łɑ��݂��Ă���̂�OPEN_EXISTING�Ƃ��܂��B
			FILE_ATTRIBUTE_NORMAL,  //�@�����F�ʐM�̏ꍇ�����͂Ȃ��̂�FILE_ATTRIBUTE_NORMAL�i�����Ȃ��j���w��@
			NULL    //�@�e���v���[�g�̃n���h���F�@�ʐM�̏ꍇ�֌W�Ȃ��@�ʏ�NULL���w��
		);
	}

	if (hPort == INVALID_HANDLE_VALUE)  //�n���h���擾�Ɏ��s�����ꍇ
	{
		CloseHandle(hPort);
		return -1;
	}


	DCB dcb;


	//�ʐM�̐ݒ�
	GetCommState(hPort, &dcb);

	dcb.DCBlength = sizeof(DCB);        //�@ DCB�̃T�C�Y�F
	//�f�[�^
	dcb.BaudRate = baudrate;                //  �`�����x�F�@�{�[���[�g��bps�P�ʂŎw��
	dcb.fBinary = TRUE;                 //    �o�C�i�����[�h�F  �ʏ�TRUE�ɐݒ�
	dcb.ByteSize = 8;                    //�@   �f�[�^�T�C�Y�@�F�@�ʏ�@8 �r�b�g
	dcb.fParity = NOPARITY;         //  �p���e�B�L����ށ@�F�@�p���e�B�Ȃ��̏ꍇ��NOPARITY���w��
	//                                     ��p���e�B�̏ꍇ�� ODDPARITY �@��
	dcb.StopBits = ONESTOPBIT;         // �X�g�b�v�r�b�g�̐��F�@�ʏ�P�r�b�g�� ONESTOPBIT;


	//�n�[�h�E�F�A�t���[����
	dcb.fOutxCtsFlow = FALSE;       // �@CTS�n�[�h�E�F�A�t���[����FCTS������g�p���Ȃ��ꍇ��FLASE���w��
	//   �@�@�@�@�@�@CTS ���������ꍇ��TRUE���w�肵��CTS�M�����Ď����܂��B                    
	dcb.fOutxDsrFlow = FALSE;       //  DSR�n�[�h�E�F�A�t���[����F�g�p���Ȃ��ꍇ��FALSE���w��
	dcb.fDtrControl = DTR_CONTROL_DISABLE;  // DTR�L��/�����F�@�����Ȃ�@DTR_CONTROL_DISABLE;ISABLE
	dcb.fRtsControl = RTS_CONTROL_DISABLE;  // RTS����F�@RTS��������Ȃ��ꍇ��RTS_CONTROL_DISABLE���w��
	//�@�@�@�@�@�@�@ RTS���������ꍇ��RTS_CONTROL_ENABLE�@�����w��
// �\�t�g�E�F�A�t���[����
	dcb.fOutX = FALSE;                 // ���M��XON/OFF����̗L���F�@�Ȃ���FLALSE 
	dcb.fInX = FALSE;                   // ��M��XON/XOFF����̗L���F�Ȃ���FALSE 
	dcb.fTXContinueOnXoff = TRUE;    // ��M�o�b�t�@�[���t��XOFF��M��̌p�����M�ہF���M��TRUE
	dcb.XonLim = 512;                 // XON��������܂łɊi�[�ł���ŏ��o�C�g���F
	dcb.XoffLim = 512;                 // XOFF��������܂łɊi�[�ł���ŏ��o�C�g���F
	dcb.XonChar = 0x11;                 // ���M��XON���� ( ���M�F�r�W�B���� ) �̎w��F
	//�@��ʂɁAXON�����Ƃ���11H ( �f�o�C�X����P�FDC1 )���悭�g���܂�
	dcb.XoffChar = 0x13;             // XOFF�����i���M�s�F�r�W�[�ʍ��j�̎w��F�Ȃ���FALSE
	//�@��ʂɁAXOFF�����Ƃ���13H ( �f�o�C�X����3�FDC3 )���悭�g���܂�


//���̑�

	dcb.fNull = TRUE;                 // NULL�o�C�g�̔j���F �j�����遨TRUE
	dcb.fAbortOnError = TRUE;         // �G���[���̓ǂݏ�������I���F�@�I�����遨TRUE
	dcb.fErrorChar = FALSE;             // �p���e�B�G���[�������̃L�����N�^�iErrorChar�j�u���F�@�Ȃ���FLALSE
	dcb.ErrorChar = 0x00;             // �p���e�B�G���[�������̒u���L�����N�^
	dcb.EofChar = 0x03;             // �f�[�^�I���ʒm�L�����N�^�@�F�@��ʂ�0x03(ETX)���悭�g���܂��B
	dcb.EvtChar = 0x02;             // �C�x���g�ʒm�L�����N�^ �F�@��ʂ�0x02(STX)���悭�g���܂�

	int Ret = 0;


	Ret = SetCommState(hPort, &dcb);  //SetCommState()�֐��Ƀ|�[�g�n���h�������dcb�\���̂̃A�h���X��������

	
	if (Ret == FALSE)  // ���s�����ꍇ
	{
		//CloseHandle(hPort);
		return -2;
	}



	Ret = SetupComm(       //�ݒ�
		hPort, // �@�ʐM�f�o�C�X�̃n���h���FCreateFile()�Ŏ擾�����n���h�����w��
		receivebuffer,   //   ��M�o�b�t�@�[�T�C�Y�F�@��M�̃o�b�t�@�[�T�C�Y���o�C�g�P�ʂŎw��
		transmitbuffer    // �@���M�o�b�t�@�[�T�C�Y�F�@���M�̃o�b�t�@�[�T�C�Y���o�C�g�P�ʂŎw��
	);
	if (Ret == FALSE)  //�@���s�����ꍇ
	{
		//CloseHandle(hPort);
		return -3;
	}

	COMMTIMEOUTS timeout;

	timeout.ReadIntervalTimeout = 50; // �����Ǎ����̂Q�����Ԃ̑S�̑҂����ԁimsec�j

	timeout.ReadTotalTimeoutMultiplier = 0; //�Ǎ��̂P����������̎���
	timeout.ReadTotalTimeoutConstant = readTimeOut; //�Ǎ��G���[���o�p�̃^�C���A�E�g����
	//(��M�g�[�^���^�C���A�E�g) = ReadTotalTimeoutMultiplier * (��M�\��o�C�g��) + ReadTotalTimeoutConstant

	timeout.WriteTotalTimeoutMultiplier = 0; //�������݂P����������̑҂�����
	timeout.WriteTotalTimeoutConstant = writeTimeOut;//�������݃G���[���o�p�̃^�C���A�E�g����
	//(���M�g�[�^���^�C���A�E�g) = WriteTotalTimeoutMultiplier * (���M�\��o�C�g��) + WriteTotalTimeoutConstant

	Ret = SetCommTimeouts(hPort, &timeout);//SetCommTimeOut()�֐��Ƀ|�[�g�n���h�������COMMTIMEOUTS�\���̂�
	//�A�h���X�������܂��B

	if (Ret == FALSE) //���s�����ꍇ
	{
		//CloseHandle(hPort);
		return -4;
	}



	*handlein = hPort;
	return 0;
}



//����M�o�b�t�@�̃N���A�����i-1���A���Ă����玸�s�j
inline int ASerial_Clearbuffer(HANDLE hPort) {
	int Ret = 0;

	Ret = PurgeComm(   //����
		hPort, // �@�ʐM�f�o�C�X�̃n���h���FCreateFile()�Ŏ擾�����n���h�����w��
		PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR
		//   ���s���鑀��F ��L�͖������̓Ǐ����̒��~�y�ё���M�̃o�b�t�@�[�̃N���A���w��
	);

	if (Ret == FALSE)  //���s�����ꍇ
	{
		//CloseHandle(hPort);
		return -1;
	}
	return 0;
}

//����M�o�b�t�@�̊m�ہi�f�t�H���g��1024�o�C�g�j
inline int ASerial_Setbuffer(HANDLE hPort, int receivebuffer = ASERIAL_RECEIVEBUFFER_NORMAL, int transmitbuffer = ASERIAL_TRANSMITBUFFER_NOMAL) {
	int Ret = 0;

	Ret = SetupComm(       //�ݒ�
		hPort, // �@�ʐM�f�o�C�X�̃n���h���FCreateFile()�Ŏ擾�����n���h�����w��
		receivebuffer,   //   ��M�o�b�t�@�[�T�C�Y�F�@��M�̃o�b�t�@�[�T�C�Y���o�C�g�P�ʂŎw��
		transmitbuffer    // �@���M�o�b�t�@�[�T�C�Y�F�@���M�̃o�b�t�@�[�T�C�Y���o�C�g�P�ʂŎw��
	);
	if (Ret == FALSE)  //�@���s�����ꍇ
	{
		//CloseHandle(hPort);
		return -1;
	}
	return 0;
}

//����M�̃^�C���A�E�g�ݒ�i�f�t�H���g�͂ǂ����500msec)
inline int ASerial_Settimeout(HANDLE hPort, int readTimeOut = ASERIAL_READTIMEOUT_NORMAL, int writeTimeOut = ASERIAL_WRITETIMEOUT_NORMAL) {
	COMMTIMEOUTS timeout;

	timeout.ReadIntervalTimeout = 500; // �����Ǎ����̂Q�����Ԃ̑S�̑҂����ԁimsec�j

	timeout.ReadTotalTimeoutMultiplier = 0; //�Ǎ��̂P����������̎���
	timeout.ReadTotalTimeoutConstant = readTimeOut; //�Ǎ��G���[���o�p�̃^�C���A�E�g����
	//(��M�g�[�^���^�C���A�E�g) = ReadTotalTimeoutMultiplier * (��M�\��o�C�g��) + ReadTotalTimeoutConstant

	timeout.WriteTotalTimeoutMultiplier = 0; //�������݂P����������̑҂�����
	timeout.WriteTotalTimeoutConstant = writeTimeOut;//�������݃G���[���o�p�̃^�C���A�E�g����
	//(���M�g�[�^���^�C���A�E�g) = WriteTotalTimeoutMultiplier * (���M�\��o�C�g��) + WriteTotalTimeoutConstant

	int Ret = 0;

	Ret = SetCommTimeouts(hPort, &timeout);//SetCommTimeOut()�֐��Ƀ|�[�g�n���h�������COMMTIMEOUTS�\���̂�
	//�A�h���X�������܂��B

	if (Ret == FALSE) //���s�����ꍇ
	{
		//CloseHandle(hPort);
		return -1;
	}


	return 0;
}

//int�^�̃f�[�^�𑗐M(���M���̂�char�^)
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

	Ret = WriteFile(    //�f�[�^�̑��M
		hPort,          // �@�ʐM�f�o�C�X�̃n���h���FCreateFile()�Ŏ擾�����n���h�����w��
		&intdata,  //�@���M�f�[�^�̃|�C���^���w��
		inbyte,                // �@���M����f�[�^�̃o�C�g�����w��
		&dwSendSize, //  ���ۂɑ��M���ꂽ�o�C�g���iDWORD)���i�[�����|�C���^���w��
		NULL          // �@�@�ʐM�Ƃ͊֌W�Ȃ������Ȃ̂�NULL���w��@�@�@�@
	);

	if (Ret == FALSE)  //���s�����ꍇ
	{
		//CloseHandle(hPort);
		return -1;
	}
	return (int)dwSendSize;
}

//char�^�̃f�[�^�𑗐M
inline int ASerial_Write(HANDLE hPort, const char* inbuf) {
	DWORD dwSendSize;
	DWORD inbyte = strlen(inbuf);

	int Ret = 0;

	Ret = WriteFile(    //�f�[�^�̑��M
		hPort,          // �@�ʐM�f�o�C�X�̃n���h���FCreateFile()�Ŏ擾�����n���h�����w��
		inbuf,  //�@���M�f�[�^�̃|�C���^���w��
		inbyte,                // �@���M����f�[�^�̃o�C�g�����w��
		&dwSendSize, //  ���ۂɑ��M���ꂽ�o�C�g���iDWORD)���i�[�����|�C���^���w��
		NULL          // �@�@�ʐM�Ƃ͊֌W�Ȃ������Ȃ̂�NULL���w��@�@�@�@
	);

	if (Ret == FALSE)  //���s�����ꍇ
	{
		//CloseHandle(hPort);
		return -1;
	}
	return (int)dwSendSize;
}

//int�^�̃f�[�^����M
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
		Ret = ReadFile(   // �f�[�^�̎�M
			hPort,   // �@�ʐM�f�o�C�X�̃n���h���F�@CreateFile()�Ŏ擾�����n���h�����w��
			&datain,       // ��M�o�b�t�@�[�̃|�C���^���w��F�@��M�f�[�^�������Ɋi�[����܂��B
			Readsize,         //�@��M����o�C�g�����w��F�@�����Ŏw�肷��o�C�g������M���邩�܂��̓^�C���A�E�g���Ԃ�����܂�
			// ReadFile()�֐��́i�@getc()�̂悤�Ɂ@�j�҂��܂�
			&dwSendSize,  //  ���ۂɎ�M�����o�C�g���iDWORD)���i�[�����|�C���^���w��
			NULL   // �ʐM�Ƃ͊֌W�Ȃ������Ȃ̂�NULL���w��
		);

		if (Ret == FALSE)     //���s�����ꍇ
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
	Ret = ReadFile(   // �f�[�^�̎�M
		hPort,   // �@�ʐM�f�o�C�X�̃n���h���F�@CreateFile()�Ŏ擾�����n���h�����w��
		&outbuf,       // ��M�o�b�t�@�[�̃|�C���^���w��F�@��M�f�[�^�������Ɋi�[����܂��B
		64,         //�@��M����o�C�g�����w��F�@�����Ŏw�肷��o�C�g������M���邩�܂��̓^�C���A�E�g���Ԃ�����܂�
		// ReadFile()�֐��́i�@getc()�̂悤�Ɂ@�j�҂��܂�
		&dwSendSize,  //  ���ۂɎ�M�����o�C�g���iDWORD)���i�[�����|�C���^���w��
		NULL   // �ʐM�Ƃ͊֌W�Ȃ������Ȃ̂�NULL���w��
	);

	if (Ret == FALSE)     //���s�����ꍇ
	{

		//CloseHandle(hPort);
		return -1;
	}


	return 0;
}

//int�^�̃f�[�^�Q�̓ǂݍ���
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
		Ret = ReadFile(   // �f�[�^�̎�M
			hPort,   // �@�ʐM�f�o�C�X�̃n���h���F�@CreateFile()�Ŏ擾�����n���h�����w��
			&inbuf,       // ��M�o�b�t�@�[�̃|�C���^���w��F�@��M�f�[�^�������Ɋi�[����܂��B
			Readsize,         //�@��M����o�C�g�����w��F�@�����Ŏw�肷��o�C�g������M���邩�܂��̓^�C���A�E�g���Ԃ�����܂�
			// ReadFile()�֐��́i�@getc()�̂悤�Ɂ@�j�҂��܂�
			&dwSendSize,  //  ���ۂɎ�M�����o�C�g���iDWORD)���i�[�����|�C���^���w��
			NULL   // �ʐM�Ƃ͊֌W�Ȃ������Ȃ̂�NULL���w��
		);

		if (Ret == FALSE)     //���s�����ꍇ
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



//�|�[�g�̃N���[�Y
inline int ASerial_Close(HANDLE hPort) {
	CloseHandle(hPort);
	return 0;
}






