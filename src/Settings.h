#ifndef XIAOZHI_AI_ARDUINO_SETTINGS_H
#define XIAOZHI_AI_ARDUINO_SETTINGS_H


#include <Arduino.h>
#include <Preferences.h>
#include <vector>
#include <map>

#define SETTINGS_NAMESPACE                  "settings"
#define SETTING_VOLUME_RATIO                "volumeRatio"
#define SETTING_SPEED_RATIO                 "speedRatio"
#define SETTING_VOICE_TYPE                  "voiceType"
#define SETTING_PERSONA                     "persona"
#define SETTING_WIFI_SSID                   "wifiSsid"
#define SETTING_WIFI_PASSWORD               "wifiPassword"
#define SETTING_SCREEN_BRIGHTNESS           "brightness"

struct WifiInfo {
    WifiInfo(String ssid, const int rssi, const bool encrypted) : _ssid(std::move(ssid)),
                                                                  _rssi(rssi),
                                                                  _encrypted(encrypted) {
    }

    String _ssid; // WiFi名称
    int _rssi; // WiFi信号强度
    bool _encrypted; // 是否需要密码
};

class Settings {
public:
    static void begin();

    static void reset();

    static void show();

    static std::vector<WifiInfo> getWifiList();

    static String getCurrentVoice();

    static int getScreenBrightness();

    static void setScreenBrightness(int brightness);

    static void setCurrentVoice(const String &voice);

    static String getCurrentPersona();

    static std::map<std::string, std::string> getVoiceMap();

    static std::map<std::string, std::string> getPersonaMap();

    static std::string getDoubaoAppId();

    static std::string getDoubaoAccessToken();

    static std::string getCozeToken();

    static void setCurrentPersona(const String &persona);

    static double getCurrentSpeakVolumeRatio();

    static void setCurrentSpeakVolumeRatio(double speakVolumeRatio);

    static double getCurrentSpeakSpeedRatio();

    static void setCurrentSpeakSpeedRatio(double speakSpeedRatio);

    static void setWifiInfo(const std::string &ssid, const std::string &password);

    static std::pair<std::string, std::string> getWifiInfo();

private:
    static String currentVoice; // 当前设置的发音人
    static String currentPersona; // 当前设置的人设
    static int currentScreenBrightness; // 当前屏幕亮度 [30, 100]
    static double currentSpeakVolumeRatio; // 当前设置的音量
    static double currentSpeakSpeedRatio; // 当前设置的说话语速

    static std::map<std::string, std::string> voiceMap; // <声音，声音值>
    static std::map<std::string, std::string> personaMap; // <人设，botId>
    static std::string doubaoAppId;
    static std::string doubaoAccessToken;
    static std::string cozeToken;
    static std::vector<WifiInfo> scannedWifiList; // 已扫描的wifi列表
    static std::string wifiSsid;
    static std::string wifiPassword;
    static Preferences preferences;
};

#endif //XIAOZHI_AI_ARDUINO_SETTINGS_H
