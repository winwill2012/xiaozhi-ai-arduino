#ifndef XIAOZHI_AI_ARDUINO_WIFIMANAGER_H
#define XIAOZHI_AI_ARDUINO_WIFIMANAGER_H
#include "misc/lv_types.h"
#include <string>

class WiFiManager {
public:
    static void setupWiFi();

    static bool setupWiFi(const char *ssid, const char *password, int maxRetries);

    static void setupWiFiWithAnim(const std::string &ssid, const std::string &password, int maxRetries);

    static bool isConnectingWifi;
    static int connectRetries;
};


#endif //XIAOZHI_AI_ARDUINO_WIFIMANAGER_H
