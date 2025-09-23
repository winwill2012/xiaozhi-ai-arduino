#include "Application.h"
#include "Arduino.h"
#include "GlobalState.h"
#include "LvglDisplay.h"
#include "Settings.h"
#include "WiFiManager.h"

static auto TAG = "Main";

void setup() {
    Settings::begin();
    LvglDisplay::begin();
    WiFiManager::setupWiFi();

    ESP_LOGD(TAG, "等待网络连接...");
    // 等待网络连接成功
    xEventGroupWaitBits(GlobalState::getEventGroup(), GlobalState::getEventBits({NetworkConnected}),
                        false, true, portMAX_DELAY);
    ESP_LOGD(TAG, "网络连接成功");
    GlobalState::setState(Sleep);
    Application::getInstance().begin();
}

void loop() {
}
