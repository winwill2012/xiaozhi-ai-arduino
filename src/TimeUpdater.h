#ifndef XIAOZHI_AI_ARDUINO_TIMEUPDATER_H
#define XIAOZHI_AI_ARDUINO_TIMEUPDATER_H

class TimeUpdater {
public:
    TimeUpdater();

    static void begin();

private :
    static unsigned long _lastMinute;
};
#endif //XIAOZHI_AI_ARDUINO_TIMEUPDATER_H
