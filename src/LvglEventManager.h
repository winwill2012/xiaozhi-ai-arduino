#ifndef XIAOZHI_AI_ARDUINO_LVGLEVENTMANAGER_H
#define XIAOZHI_AI_ARDUINO_LVGLEVENTMANAGER_H

class LvglEventManager {
public:
    static void registerLvglEventCallback();

    static bool isRefreshingWiFi;
    static int refreshImageAngle;
};


#endif //XIAOZHI_AI_ARDUINO_LVGLEVENTMANAGER_H
