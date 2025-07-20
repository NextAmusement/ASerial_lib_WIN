// ASerial for Windows powerd by WindowsAPI
// Controller mode only

// include
#include "ASerial_core/ASerial_packet.h"
#include "WindowsSerial/WindowsSerial.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>



#ifndef ASERIAL_LIB_CONTROLLER_WIN_H
#define ASERIAL_LIB_CONTROLLER_WIN_H

class ASerial_lib_Controller_Win : public ASerialPacket {
 public:
    //  @brief  ASerialオブジェクトを生成する
    //  @param  target_device_id  ターゲットデバイスID
    //  @param  baudrate    ボーレート(デフォルト115200bps)*省略可
    ASerial_lib_Controller_Win(int target_device_id, int device_ver);

    ASerial_lib_Controller_Win(int target_device_id, int device_ver_min, int device_ver_max);

    int ConnectDevice(int COM_num);

    int AutoConnectDevice(void);

    int DisConnectDevice(void);

    void SetInterfacePt(WindowsSerial *interface_pt);

    int ReadDataProcess(ASerialDataStruct::ASerialData *read_data_buf);

    int WriteData(uint8_t command, uint8_t *data, uint8_t data_num);

    int WriteData(uint8_t command);

    void DeviceReset(void);

 private:
    WindowsSerial *m_inteface = nullptr;
    int m_target_device_id = 0;
    int m_device_ver_min = 0;
    int m_device_ver_max = 0;
};

#endif  // ASERIAL_LIB_CONTROLLER_WIN_H