#ifndef IOT_H
#define IOT_H

#include "Arduino.h"
#include "FastLED.h"

#define RGB_PIN    18

class IOT
{
public:
    static void begin();
    // 处理智能控制请求
    static void handle(const String& command, const String& parameter);
    // 修改RGB灯光颜色
    static void changeRgbColor(uint32_t color);
    // 修改RGB等亮度
    static void changeRgbBrightness(int brightness);
    // 开灯
    static void turnOnRgb();
    // 关灯
    static void turnOffRgb();
    // 调节屏幕亮度
    static void changeScreenBrightness(int brightness);
    // 调整音量
    static void changeVolume(int volume);
    // 调整音色
    static void changeVoice(const String& voice);
    // 调整说话语速
    static void changeSpeakSpeed(double speed);

private:
    static CRGB leds[1];
};


#endif //IOT_H
