#include "WiFiManager.h"
#include "WiFi.h"
#include "Arduino.h"
#include "GlobalState.h"
#include "Settings.h"
#include "lvgl.h"
#include "ui/gui.h"

static auto TAG = "WiFiManager";

bool WiFiManager::isConnectingWifi = false;
int WiFiManager::connectRetries = 0;

void WiFiManager::begin() {
    const auto wifiInfo = Settings::getWifiInfo();
    if (wifiInfo.first.empty() || wifiInfo.second.empty()) {
        ESP_LOGI(TAG, "未发现可用WiFi配置");
        GlobalState::setState(NetworkConfigurationNotFound);
    } else {
        beginWithAnim(wifiInfo.first.c_str(), wifiInfo.second.c_str(), 20);
    }
}

void WiFiManager::beginWithAnim(const char *ssid, const char *password, int maxRetries) {
    lv_timer_create([](lv_timer_t *timer) {
        const bool completed = begin(lv_ui.clicked_wifi_name,
                                     lv_textarea_get_text(lv_ui.menu_network_page_password_textarea),
                                     10);
        if (completed) {
            lv_timer_delete(timer);
            lv_obj_add_flag(lv_ui.menu_network_page_keyboard_mask, LV_OBJ_FLAG_HIDDEN);
        }
    }, 500, nullptr);
}

bool WiFiManager::begin(const char *ssid, const char *password, int maxRetries) {
    if (!isConnectingWifi) {
        ESP_LOGI(TAG, "使用WiFi信息连接WiFI: %s, %s", ssid, password);
        WiFi.disconnect(false, true);
        GlobalState::setState(NetworkConnecting);
        WiFiClass::mode(WIFI_MODE_STA);
        WiFiClass::useStaticBuffers(true);
        WiFi.begin(ssid, password);
        isConnectingWifi = true;
        return false;
    }
    if (WiFiClass::status() == WL_CONNECTED) {
        GlobalState::setState(NetworkConnected);
        isConnectingWifi = false;
        connectRetries = 0;
        return true;
    }
    connectRetries++;
    ESP_LOGI(TAG, "WiFi连接，尝试次数：%d, 最大尝试次数: %d", connectRetries, maxRetries);
    if (WiFiClass::status() != WL_CONNECTED) {
        GlobalState::setState(NetworkConnecting);
        if (connectRetries >= maxRetries) {
            ESP_LOGW(TAG, "WiFi连接超时");
            WiFi.disconnect(false, true);
            GlobalState::setState(NetworkConnectFailed);
            isConnectingWifi = false;
            connectRetries = 0;
            return true;
        }
        return false;
    }
    return false;
}

bool WiFiManager::reconnectWifi(lv_timer_t *timer, const char *ssid, const char *password, const int maxRetries) {
    // if (isConnectingWifi) {
    //     connectRetries++;
    //     GlobalState::setState(NetworkConnecting);
    //     if (WiFi.isConnected()) {
    //         GlobalState::setState(NetworkConnected);
    //         lv_timer_delete(timer); // 连接成功，删除联网定时器
    //         lv_screen_load(guider_ui.screen_main);
    //         isConnectingWifi = false;
    //         connectRetries = 0;
    //         return true;
    //     }
    //     if (connectRetries >= maxRetries) {
    //         GlobalState::setState(NetworkConnectFailed);
    //         lv_timer_delete(timer); // 重试次数超过限制，联网失败，删除联网定时器
    //         isConnectingWifi = false;
    //         connectRetries = 0;
    //         return false;
    //     }
    //     return false;
    // }
    // isConnectingWifi = true;
    // GlobalState::setState(NetworkConnecting);
    // if (WiFi.isConnected()) {
    //     WiFi.disconnect(true, true);
    // }
    // log_d("connect wifi with config: [%s, %s]", ssid, password);
    // WiFiClass::mode(WIFI_MODE_STA);
    // WiFiClass::useStaticBuffers(true);
    // WiFi.begin(ssid, password);
    return false;
}
