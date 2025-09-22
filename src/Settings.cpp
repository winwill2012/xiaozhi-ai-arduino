#include "Settings.h"
#include "SPIFFS.h"
#include <WiFi.h>
#include <map>
#include "ArduinoJson.h"
#include "GlobalState.h"

Preferences Settings::preferences;
String Settings::currentVoice;
String Settings::currentPersona;
int Settings::currentScreenBrightness;
double Settings::currentSpeakSpeedRatio;
double Settings::currentSpeakVolumeRatio;
std::map<std::string, std::string> Settings::voiceMap = std::map<std::string, std::string>(); // <声音，声音值>列表
std::map<std::string, std::string> Settings::personaMap = std::map<std::string, std::string>(); // <人设，botId>列表
std::string Settings::doubaoAppId;
std::string Settings::doubaoAccessToken;
std::string Settings::cozeToken;
std::string Settings::wifiSsid;
std::string Settings::wifiPassword;
std::vector<WifiInfo> Settings::scannedWifiList = std::vector<WifiInfo>();

void Settings::begin() {
    if (!SPIFFS.begin(true)) {
        log_e("an Error has occurred while mounting SPIFFS");
        ESP.restart();
    }
    File file = SPIFFS.open("/settings.json", "r");
    if (!file) {
        log_e("failed to open settings.json file for reading");
        ESP.restart();
    }
    JsonDocument doc;
    const DeserializationError error = deserializeJson(doc, file);
    file.close();
    if (error) {
        log_e("deserialize settings.json failed: %s", error.c_str());
        ESP.restart();
    }

    preferences.begin(SETTINGS_NAMESPACE, false);
    currentSpeakVolumeRatio = preferences.getDouble(SETTING_VOLUME_RATIO, doc["system"]["volumeRatio"].as<double>());
    currentSpeakSpeedRatio = preferences.getDouble(SETTING_SPEED_RATIO, doc["system"]["speakSpeed"].as<double>());
    currentVoice = preferences.getString(SETTING_VOICE_TYPE, doc["system"]["voiceType"].as<String>());
    currentPersona = preferences.getString(SETTING_PERSONA, doc["system"]["persona"].as<String>());
    wifiSsid = preferences.getString(SETTING_WIFI_SSID, "").c_str();
    wifiPassword = preferences.getString(SETTING_WIFI_PASSWORD, "").c_str();
    currentScreenBrightness = preferences.getInt(SETTING_SCREEN_BRIGHTNESS, 80);
    preferences.end();

    doubaoAppId = doc["doubao"]["appId"].as<std::string>();
    doubaoAccessToken = doc["doubao"]["accessToken"].as<std::string>();
    cozeToken = doc["coze"]["token"].as<std::string>();
    const JsonArray voiceListJsonArray = doc["voiceList"].as<JsonArray>();
    for (JsonObject item: voiceListJsonArray) {
        voiceMap[item["name"].as<std::string>()] = item["value"].as<std::string>();
    }
    const JsonArray personaListJsonArray = doc["coze"]["personaList"].as<JsonArray>();
    for (JsonObject item: personaListJsonArray) {
        personaMap[item["name"].as<std::string>()] = item["botId"].as<std::string>();
    }
    show();
}

void Settings::reset() {
    preferences.begin(SETTINGS_NAMESPACE, false);
    preferences.remove(SETTING_VOLUME_RATIO);
    preferences.remove(SETTING_SPEED_RATIO);
    preferences.remove(SETTING_VOICE_TYPE);
    preferences.remove(SETTING_PERSONA);
    preferences.remove(SETTING_WIFI_SSID);
    preferences.remove(SETTING_WIFI_PASSWORD);
    preferences.remove(SETTING_SCREEN_BRIGHTNESS);
    preferences.end();
    ESP.restart();
}

void Settings::show() {
    ESP_LOGI("Settings", "------------------------当前系统配置---------------------------------");
    ESP_LOGI("Settings", "  当前音量: %f", currentSpeakVolumeRatio);
    ESP_LOGI("Settings", "  当前语速: %f", currentSpeakSpeedRatio);
    ESP_LOGI("Settings", "  当前音色: %s", currentVoice.c_str());
    ESP_LOGI("Settings", "  当前性格: %s", currentPersona.c_str());
    ESP_LOGI("Settings", "豆包AppId: %s", doubaoAppId.c_str());
    ESP_LOGI("Settings", "豆包Token: %s", doubaoAccessToken.c_str());
    ESP_LOGI("Settings", "扣子Token: %s", cozeToken.c_str());
    ESP_LOGI("Settings", "WiFi Sid: %s", wifiSsid.c_str());
    ESP_LOGI("Settings", " WiFi密码: %s", wifiPassword.c_str());
    ESP_LOGI("Settings", "-------------------------------------------------------------------");
}

/**
*  扫描设备周围WiFi列表
*/
std::vector<WifiInfo> Settings::getWifiList() {
    ESP_LOGD("Settings", "开始扫描周边WiFi列表...");
    const int16_t number = WiFi.scanNetworks(true);
    if (number == 0) {
        return {};
    }
    ESP_LOGD("Settings", "周边WiFi扫描结束, 一共 %d 个AP", number);
    scannedWifiList.clear();
    for (int i = 0; i < number; i++) {
        scannedWifiList.emplace_back(WiFi.SSID(i), WiFi.RSSI(i), WiFi.encryptionType(i) != WIFI_AUTH_OPEN);
    }
    return scannedWifiList;
}

String Settings::getCurrentVoice() {
    return currentVoice;
}


void Settings::setCurrentVoice(const String &voice) {
    currentVoice = voice;
    preferences.begin(SETTINGS_NAMESPACE);
    preferences.putString(SETTING_VOICE_TYPE, voice);
    preferences.end();
}

String Settings::getCurrentPersona() {
    return currentPersona;
}

void Settings::setCurrentPersona(const String &persona) {
    currentPersona = persona;
    preferences.begin(SETTINGS_NAMESPACE);
    preferences.putString(SETTING_PERSONA, persona);
    preferences.end();
}

double Settings::getCurrentSpeakVolumeRatio() {
    return currentSpeakVolumeRatio;
}

void Settings::setCurrentSpeakVolumeRatio(const double speakVolumeRatio) {
    currentSpeakVolumeRatio = speakVolumeRatio;
    preferences.begin(SETTINGS_NAMESPACE);
    preferences.putDouble(SETTING_VOLUME_RATIO, speakVolumeRatio);
    preferences.end();
}

double Settings::getCurrentSpeakSpeedRatio() {
    return currentSpeakSpeedRatio;
}

void Settings::setCurrentSpeakSpeedRatio(const double speakSpeedRatio) {
    currentSpeakSpeedRatio = speakSpeedRatio;
    preferences.begin(SETTINGS_NAMESPACE);
    preferences.putDouble(SETTING_SPEED_RATIO, speakSpeedRatio);
    preferences.end();
}

void Settings::setWifiInfo(const std::string &ssid, const std::string &password) {
    wifiSsid = ssid;
    wifiPassword = password;
    preferences.begin(SETTINGS_NAMESPACE);
    preferences.putString(SETTING_WIFI_SSID, ssid.c_str());
    preferences.putString(SETTING_WIFI_PASSWORD, password.c_str());
    preferences.end();
}

std::pair<std::string, std::string> Settings::getWifiInfo() {
    return std::make_pair(wifiSsid, wifiPassword);
}

std::map<std::string, std::string> Settings::getVoiceMap() {
    return voiceMap;
}

std::map<std::string, std::string> Settings::getPersonaMap() {
    return personaMap;
}

std::string Settings::getDoubaoAppId() {
    return doubaoAppId;
}

std::string Settings::getDoubaoAccessToken() {
    return doubaoAccessToken;
}

std::string Settings::getCozeToken() {
    return cozeToken;
}

int Settings::getScreenBrightness() {
    return currentScreenBrightness;
}

void Settings::setScreenBrightness(int brightness) {
    currentScreenBrightness = brightness;
    analogWrite(8, static_cast<int>(currentScreenBrightness * 2.55));
    preferences.begin(SETTINGS_NAMESPACE);
    preferences.putInt(SETTING_SCREEN_BRIGHTNESS, brightness);
    preferences.end();
}
