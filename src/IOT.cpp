#include "IOT.h"
#include "FastLED.h"
#include "Settings.h"
#include "Utils.h"

CRGB IOT::leds[1];

void IOT::begin()
{
    FastLED.addLeds<WS2812, RGB_PIN, GRB>(leds, 1);
}

void IOT::handle(const String& command, const String& parameter)
{
    ESP_LOGI("IOT", "执行智能控制: %s, %s", command.c_str(), parameter.c_str());
    if (command == "switch")
    {
        if (parameter.indexOf("on") >= 0)
        {
            turnOnRgb();
        }
        else if (parameter.indexOf("off") >= 0)
        {
            turnOffRgb();
        }
    }
    else if (command == "color")
    {
        changeRgbColor(hexColorToUInt(parameter));
    }
    else if (command == "brightness")
    {
        changeRgbBrightness(str2int(parameter));
    }
    else if (command == "background")
    {
        changeScreenBrightness(str2int(parameter));
    }
    else if (command == "volume")
    {
        changeVolume(str2int(parameter));
    }
    else if (command == "voice")
    {
        changeVoice(parameter);
    }
    else if (command == "speed")
    {
        changeSpeakSpeed(str2double(parameter));
    }
}

void IOT::changeRgbColor(const uint32_t color)
{
    leds[0] = color;
    FastLED.show();
}

void IOT::changeRgbBrightness(const int brightness)
{
    FastLED.setBrightness(brightness);
    FastLED.show();
}

void IOT::turnOnRgb()
{
    FastLED.setBrightness(50);
    leds[0] = CRGB::LightGreen;
    FastLED.show();
}

void IOT::turnOffRgb()
{
    FastLED.setBrightness(0);
    FastLED.show();
}

void IOT::changeScreenBrightness(const int brightness)
{
    Settings::setScreenBrightness(brightness);
}

void IOT::changeVolume(const int volume)
{
    Settings::setCurrentSpeakVolumeRatio(volume * 1.0 / 100);
}

void IOT::changeVoice(const String& voice)
{
    Settings::setCurrentVoice(voice);
}

void IOT::changeSpeakSpeed(const double speed)
{
    Settings::setCurrentSpeakSpeedRatio(speed);
}
