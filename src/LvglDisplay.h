#ifndef XIAOZHI_AI_ARDUINO_LVGLDISPLAY_H
#define XIAOZHI_AI_ARDUINO_LVGLDISPLAY_H

#include "Arduino.h"
#include "misc/lv_types.h"

class LvglDisplay {
public:
    static void begin();

    static void loadSettings();

    static void updateTime(const std::string &time);

    static void updateState(const std::string &state);

    static void updateChatText(bool newLine, const std::string &text);

    static void forbidSpeak(bool forbid);

    static SemaphoreHandle_t lvglUpdateLock;

private :
    static lv_obj_t *last_message;
    static int current_message_number;

    static void registerLvglEventCallback();
};


#endif //XIAOZHI_AI_ARDUINO_LVGLDISPLAY_H
