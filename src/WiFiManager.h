#ifndef XIAOZHI_AI_ARDUINO_WIFIMANAGER_H
#define XIAOZHI_AI_ARDUINO_WIFIMANAGER_H
#include "misc/lv_types.h"

class WiFiManager {
public:
    static void begin();

    static bool begin(const char *ssid, const char *password, int maxRetries);

    static void beginWithAnim(const char *ssid, const char *password, int maxRetries);

    static bool reconnectWifi(lv_timer_t *timer, const char *ssid, const char *password, int maxRetries);

    static bool isConnectingWifi;
    static int connectRetries;
};


#endif //XIAOZHI_AI_ARDUINO_WIFIMANAGER_H
