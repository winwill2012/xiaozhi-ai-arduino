#ifndef XIAOZHI_AI_ARDUINO_LVGLDISPLAY_H
#define XIAOZHI_AI_ARDUINO_LVGLDISPLAY_H

#include "Arduino.h"

class LvglDisplay {
public:
    static void begin();

    static void updateTime(const std::string &time);

    static void updateState(const std::string &state);

    static SemaphoreHandle_t lvglUpdateLock;

private :
    static void registerLvglEventCallback();
};


#endif //XIAOZHI_AI_ARDUINO_LVGLDISPLAY_H
