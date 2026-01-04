#ifndef ASERIAL_LIB_CONTROLLER_WIN_MT_H
#define ASERIAL_LIB_CONTROLLER_WIN_MT_H
#include <thread>
#include <functional>
#include <string>
#include "ASerial_lib_Controller_Win.h"

class ASerial_lib_Controller_Win_MT : public ASerial_lib_Controller_Win {
 public:
    enum DEBUGLOG_SETTING {
        DEBUGLOG_OFF = 0,
        DEBUGLOG_ON_NORMAL = 1, //通常ログ出力
        DEBUGLOG_ON_ALL = 2     //詳細ログ出力
    };
    enum AUTO_CONNECT_THREAD_STATE {
        AUTO_CONNECT_THREAD_END = 0,    //正常終了
        AUTO_CONNECT_THREAD_RAN = 1,    //
        AUTO_CONNECT_THREAD_ERROR = 2,
        AUTO_CONNECT_THREAD_STANDBY = 3
    };

    ///  @brief ASerialオブジェクトを生成する(マルチスレッド対応版)
    ///  @param target_device_id  ターゲットデバイスID
    ///  @param device_ver デバイスのバージョン
    ASerial_lib_Controller_Win_MT(int target_device_id, int device_ver);

    /// @brief ASerialオブジェクトを生成する(マルチスレッド対応版)
    /// @param target_device_id ターゲットデバイスID
    /// @param device_ver_min  デバイスの最小バージョン
    /// @param device_ver_max  デバイスの最大バージョン
    ASerial_lib_Controller_Win_MT(int target_device_id, int device_ver_min, int device_ver_max);

    /// @brief デストラクタ
    ~ASerial_lib_Controller_Win_MT();

    /// @brief 自動接続スレッドを作成する
    /// @return 0:スレッド作成成功 -1:スレッド作成失敗
    int MakeAutoConnectThread(void);

    /// @brief 自動接続スレッドを削除する
    /// @return 0:スレッド削除成功 -1:スレッド削除失敗
    int DeleteAutoConnectThread(void);

    /// @brief デバッグログ設定を行う
    /// @param setting デバッグログ設定(DEBUGLOG_SETTING参照)
    void SetDebugLogSetting(unsigned int setting);

    /// @brief ログ出力関数を設定する
    /// @param log_output_func ログ出力関数(std::function<void(std::string)>型)
    void SetLogOutputFunc(std::function<void(std::string)> log_output_func);

    /// @brief 自動接続スレッドを開始する
    /// @return 0:スレッド開始成功 -1:スレッド開始失敗
    int StartAutoConnect(void);

    /// @brief 自動接続スレッドの状態を取得する
    /// @return AUTO_CONNECT_THREAD_STATE参照
    int GetAutoConnectThreadState(void);

    /// @brief 接続されたCOM番号を取得する 
    /// @return -1:接続されていない、-1以外:接続されたCOM番号
    int GetConnectComNum(void);



 private:
    enum THREAD_STATE {
        THREAD_STOP = 0,
        THREAD_RAN = 1,
        THREAD_END = 2
    };
    
    std::thread* auto_connect_thread_handle_ = nullptr;
    std::thread* thread_handle_ = nullptr;
    std::function<void(std::string)> LogOutput_ = nullptr;
    unsigned int thread_state_ = THREAD_STOP;
    unsigned int old_thread_state_ = THREAD_STOP;
    unsigned int auto_connect_thread_state_ = AUTO_CONNECT_THREAD_STANDBY;
    unsigned int auto_connect_com_num_ = 0;
    unsigned int debuglog_setting_ = 0;

    void ThreadMain(void);

    void AutoConnectThread(void);

    void SetThreadState(unsigned int new_state);

    /// @brief 詳細ログ出力
    /// @param log_str ログ文字列
    void LogOutput_ALLLOG(std::string log_str);

    /// @brief 通常ログ出力
    /// @param log_str ログ文字列
    void LogOutput_NORMALLOG(std::string log_str);
};

#endif  // ASERIAL_LIB_CONTROLLER_WIN_MT_H