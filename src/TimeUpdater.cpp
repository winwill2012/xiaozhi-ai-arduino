#include "TimeUpdater.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "LvglDisplay.h"

unsigned long TimeUpdater::_lastMinute = -1;
WiFiUDP udp;
auto timeClient = NTPClient(udp, "time1.aliyun.com");

void TimeUpdater::begin() {
    timeClient.begin();
    timeClient.setTimeOffset(28800); // 设置时区，这里是北京时间（UTC+8），8 * 60 * 60 = 28800
    xTaskCreate([](void *ptr) {
        while (true) {
            timeClient.update(); // update函数内部做了更新间隔检测，并不会每一次都从网络获取时间
            const unsigned long localEpochTime = timeClient.getEpochTime();
            const unsigned long currentHour = (localEpochTime % 86400) / 3600;
            const unsigned long currentMinute = (localEpochTime % 3600) / 60;
            if (currentMinute != _lastMinute) {
                char timeStr[6];
                snprintf(timeStr, sizeof(timeStr), "%02lu:%02lu", currentHour, currentMinute);
                LvglDisplay::updateTime(timeStr);
                _lastMinute = currentMinute;
            }
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }, "timeUpdater", 1024 * 4, nullptr, 1, nullptr);
}
