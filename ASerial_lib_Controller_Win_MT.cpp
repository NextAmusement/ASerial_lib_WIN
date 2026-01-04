#include "ASerial_lib_Controller_Win_MT.h"

ASerial_lib_Controller_Win_MT::ASerial_lib_Controller_Win_MT(int target_device_id, int device_ver)
    : ASerial_lib_Controller_Win(target_device_id, device_ver)
{
}

ASerial_lib_Controller_Win_MT::ASerial_lib_Controller_Win_MT(int target_device_id,
                                                             int device_ver_min,
                                                             int device_ver_max)
    : ASerial_lib_Controller_Win(target_device_id, device_ver_min, device_ver_max)
{
}

ASerial_lib_Controller_Win_MT::~ASerial_lib_Controller_Win_MT() { DeleteAutoConnectThread(); }

int ASerial_lib_Controller_Win_MT::MakeAutoConnectThread(void)
{
    LogOutput_NORMALLOG("[ASerial_thread] Creating auto connect thread...\n");

    if (auto_connect_thread_handle_ != nullptr) {
        LogOutput_ALLLOG("[ASerial_thread] Auto connect thread already exists.\n");
        return -1;
    }

    auto_connect_thread_handle_ = new std::thread(&ASerial_lib_Controller_Win_MT::AutoConnectThread, this);

    LogOutput_NORMALLOG("[ASerial_thread] Auto connect thread created.\n");
    return 0;
}

int ASerial_lib_Controller_Win_MT::DeleteAutoConnectThread(void)
{
    if (auto_connect_thread_handle_ == nullptr) {
        LogOutput_ALLLOG("[ASerial_thread] Auto connect thread is nullptr.\n");
        return -1;
    }

    LogOutput_NORMALLOG("[ASerial_thread] Deleting auto connect thread...\n");

    if (auto_connect_thread_handle_->joinable() == true) {
        LogOutput_ALLLOG("[ASerial_thread] Joining auto connect thread...\n");
        auto_connect_thread_handle_->join();
        LogOutput_ALLLOG("[ASerial_thread] Auto connect thread joined.\n");
    }

    delete auto_connect_thread_handle_;
    auto_connect_thread_handle_ = nullptr;

    LogOutput_NORMALLOG("[ASerial_thread] Auto connect thread deleted.\n");

    return 0;
}

void ASerial_lib_Controller_Win_MT::SetDebugLogSetting(unsigned int setting) { debuglog_setting_ = setting; }

void ASerial_lib_Controller_Win_MT::SetLogOutputFunc(std::function<void(std::string)> log_output_func)
{
    LogOutput_ = log_output_func;
}

int ASerial_lib_Controller_Win_MT::StartAutoConnect(void)
{
    LogOutput_NORMALLOG("[ASerial_thread] Starting auto connect...\n");

    if (thread_state_ != THREAD_STOP || GetConnectionState() == true) {
        LogOutput_ALLLOG("[ASerial_thread] Cannot start auto connect thread.\n");
        return -1;
    }

    auto_connect_thread_state_ = AUTO_CONNECT_THREAD_RAN;

    LogOutput_NORMALLOG("[ASerial_thread] Auto connect started.\n");
    return 0;
}

int ASerial_lib_Controller_Win_MT::GetAutoConnectThreadState(void)
{
    return static_cast<int>(auto_connect_thread_state_);
}

int ASerial_lib_Controller_Win_MT::GetConnectComNum(void)
{
    if (GetConnectionState() == false) {
        return -1;
    }

    return static_cast<int>(auto_connect_com_num_);
}

void ASerial_lib_Controller_Win_MT::ThreadMain(void)
{
    switch (thread_state_) {
        case THREAD_STOP: {
            break;
        }
        default:
            break;
    }
}

void ASerial_lib_Controller_Win_MT::AutoConnectThread(void)
{
    LogOutput_NORMALLOG("[ASerial_thread] Auto connect thread Ran.\n");
    if (GetConnectionState() == true) {
        LogOutput_ALLLOG("[ASerial_thread] Auto connect thread Error: Already connected.\n");
        auto_connect_thread_state_ = AUTO_CONNECT_THREAD_ERROR;
        return;
    }

    auto_connect_thread_state_ = AUTO_CONNECT_THREAD_STANDBY;
    LogOutput_ALLLOG("[ASerial_thread] Auto connect thread standby OK\n");
    int i = 1;

    while (auto_connect_thread_state_ != AUTO_CONNECT_THREAD_END) {
        switch (auto_connect_thread_state_) {
            case AUTO_CONNECT_THREAD_STANDBY: {
                break;
            }
            case AUTO_CONNECT_THREAD_RAN: {
                LogOutput_ALLLOG("[ASerial_thread] Auto connect thread: Trying to COM " + std::to_string(i) + ".\n");
                int st = ConnectDevice(i);

                if (st == -1) {
                    ++i;
                    if (i > 255) {
                        LogOutput_NORMALLOG("[ASerial_thread] Auto connect thread Error: No device found.\n");
                        auto_connect_thread_state_ = AUTO_CONNECT_THREAD_ERROR;
                        break;
                    }
                }
                else {
                    LogOutput_NORMALLOG("[ASerial_thread] Auto connect thread: Device connected on COM " + std::to_string(i) + ".\n");
                    auto_connect_com_num_ = i;
                    auto_connect_thread_state_ = AUTO_CONNECT_THREAD_END;
                }

                break;
            }
            default: {
                break;
            }
        }

        if (auto_connect_thread_state_ == AUTO_CONNECT_THREAD_ERROR) {
            LogOutput_NORMALLOG("[ASerial_thread] Auto connect thread Error.\n");
            break;
        }
    }

    LogOutput_NORMALLOG("[ASerial_thread] Auto connect thread End.\n");
}

void ASerial_lib_Controller_Win_MT::SetThreadState(unsigned int new_state)
{
    old_thread_state_ = thread_state_;
    thread_state_ = new_state;
}

void ASerial_lib_Controller_Win_MT::LogOutput_ALLLOG(std::string log_str)
{
    if (debuglog_setting_ == DEBUGLOG_ON_ALL && LogOutput_ != nullptr) {
        LogOutput_(log_str);
    }
}

void ASerial_lib_Controller_Win_MT::LogOutput_NORMALLOG(std::string log_str)
{
    if (debuglog_setting_ != DEBUGLOG_OFF && LogOutput_ != nullptr) {
        LogOutput_(log_str);
    }
}
