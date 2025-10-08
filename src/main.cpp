#include "Application.h"
#include "Arduino.h"
#include "GlobalState.h"
#include "LvglDisplay.h"
#include "Settings.h"
#include "WiFiManager.h"

static auto TAG = "Main";

void setup() {
    // 加载本地配置信息
    Settings::begin();
    // 显示屏幕
    LvglDisplay::begin();
    // 连接网络
    WiFiManager::setupWiFi();

    ESP_LOGD(TAG, "等待网络连接...");
    // 等待网络连接成功
    xEventGroupWaitBits(GlobalState::getEventGroup(), GlobalState::getEventBits({NetworkConnected}),
                        false, true, portMAX_DELAY);
    ESP_LOGD(TAG, "网络连接成功");
    GlobalState::setState(Sleep);
    // 启动聊天主流程
    Application::getInstance().begin();
}

void loop() {
}
