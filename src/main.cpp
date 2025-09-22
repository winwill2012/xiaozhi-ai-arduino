#include "Arduino.h"
#include "LvglDisplay.h"
#include "Settings.h"
#include "TimeUpdater.h"
#include "WiFiManager.h"

void setup() {
    Settings::begin();
    Settings::setWifiInfo("ChinaNet-GdPt", "19910226");
    LvglDisplay::begin();
    WiFiManager::begin();
    TimeUpdater::begin();
}

void loop() {
}
