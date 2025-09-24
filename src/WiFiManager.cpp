#include "WiFiManager.h"
#include "WiFi.h"
#include "Arduino.h"
#include "GlobalState.h"
#include "Settings.h"
#include "lvgl.h"
#include "LvglDisplay.h"
#include "TimeUpdater.h"
#include "ui/gui.h"

static auto TAG = "WiFiManager";

bool WiFiManager::isConnectingWifi = false;
int WiFiManager::connectRetries = 0;

struct WiFiSetupParams {
    std::string ssid;
    std::string password;
    int maxRetries;
};

void WiFiManager::setupWiFi() {
    const auto wifiInfo = Settings::getWifiInfo();
    if (wifiInfo.first.empty() || wifiInfo.second.empty()) {
        ESP_LOGI(TAG, "未发现可用WiFi配置");
        GlobalState::setState(NetworkConfigurationNotFound);
    } else {
        ESP_LOGI(TAG, "发现本地WiFi配置：%s, %s", wifiInfo.first.c_str(), wifiInfo.second.c_str());
        setupWiFiWithAnim(wifiInfo.first, wifiInfo.second, 20);
    }
}

void WiFiManager::setupWiFiWithAnim(const std::string &ssid, const std::string &password, int maxRetries) {
    auto *params = new WiFiSetupParams{ssid, password, maxRetries};
    lv_timer_create([](lv_timer_t *timer) {
        const auto *p = static_cast<WiFiSetupParams *>(lv_timer_get_user_data(timer));
        const bool completed = setupWiFi(p->ssid.c_str(), p->password.c_str(), p->maxRetries);
        if (completed) {
            lv_timer_delete(timer);
            lv_obj_add_flag(lv_ui.menu_network_page_keyboard_mask, LV_OBJ_FLAG_HIDDEN);
        }
    }, 500, params);
}

bool WiFiManager::setupWiFi(const char *ssid, const char *password, int maxRetries) {
    if (!isConnectingWifi) {
        isConnectingWifi = true;
        ESP_LOGI(TAG, "使用WiFi信息连接WiFI: %s, %s", ssid, password);
        GlobalState::setState(NetworkConnecting);
        WiFiClass::mode(WIFI_MODE_STA);
        WiFiClass::useStaticBuffers(true);
        WiFi.begin(ssid, password);
        return false;
    }
    if (WiFiClass::status() == WL_CONNECTED) {
        ESP_LOGI(TAG, "WiFi连接成功: %s, %s", ssid, password);
        GlobalState::setState(NetworkConnected);
        LvglDisplay::forbidSpeak(false);
        lv_image_set_src(lv_ui.status_bar_wifi_image, LV_CUSTOM_SYMBOL_WIFI);
        Settings::setWifiInfo(ssid, password); // 保存密码，下次直接连接
        TimeUpdater::begin(); // 联网成功，启动时间更新任务
        isConnectingWifi = false;
        connectRetries = 0;
        return true;
    }
    connectRetries++;
    if (WiFiClass::status() != WL_CONNECTED) {
        GlobalState::setState(NetworkConnecting);
        if (connectRetries >= maxRetries) {
            ESP_LOGW(TAG, "WiFi连接超时");
            WiFi.disconnect(false, true);
            GlobalState::setState(NetworkConnectFailed);
            LvglDisplay::forbidSpeak(true);
            lv_image_set_src(lv_ui.status_bar_wifi_image, LV_CUSTOM_SYMBOL_NO_WIFI);
            isConnectingWifi = false;
            connectRetries = 0;
            return true;
        }
        return false;
    }
    return false;
}
