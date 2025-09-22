#include "LvglEventManager.h"
#include "lvgl.h"
#include "WiFi.h"
#include "WiFiManager.h"
#include "ui/gui.h"
bool LvglEventManager::isRefreshingWiFi = false;
int LvglEventManager::refreshImageAngle = 0;

static auto TAG = "LvglEventManager";

void network_setting_password_dialog_confirm_button_cb(lv_event_t *event) {
    if (lv_event_get_code(event) == LV_EVENT_CLICKED) {
        WiFiManager::beginWithAnim(lv_ui.clicked_wifi_name,
                                   lv_textarea_get_text(lv_ui.menu_network_page_password_textarea), 20);
    }
}

void hide_wifi_password_dialog_cb(lv_event_t *event) {
    if (lv_event_get_code(event) == LV_EVENT_CLICKED) {
        lv_obj_add_flag(lv_ui.menu_network_page_keyboard_mask, LV_OBJ_FLAG_HIDDEN);
    }
}

void network_setting_wifi_item_click_cb(lv_event_t *event) {
    if (lv_event_get_code(event) == LV_EVENT_CLICKED) {
        lv_obj_t *wifi_item = static_cast<lv_obj_t *>(lv_event_get_target(event));
        lv_ui.clicked_wifi_name = lv_list_get_button_text(lv_ui.menu_network_page_wifi_list, wifi_item);
        lv_label_set_text(lv_ui.menu_network_page_password_tips_label, lv_ui.clicked_wifi_name);
        lv_textarea_set_text(lv_ui.menu_network_page_password_textarea, "");
        lv_obj_remove_flag(lv_ui.menu_network_page_keyboard_mask, LV_OBJ_FLAG_HIDDEN);
    }
}

void network_setting_refresh_wifi_button_cb(lv_event_t *event) {
    if (lv_event_get_code(event) == LV_EVENT_CLICKED) {
        // 当前没有在刷新wifi列表，则扫描wifi列表
        if (!LvglEventManager::isRefreshingWiFi) {
            // 清空列表
            auto child = lv_obj_get_child(lv_ui.menu_network_page_wifi_list, 0);
            while (child != nullptr) {
                lv_obj_delete(child);
                child = lv_obj_get_child(lv_ui.menu_network_page_wifi_list, 0);
            }
            // 开始新的扫描
            lv_obj_remove_flag(lv_ui.menu_network_page_wifi_list_loading_image, LV_OBJ_FLAG_HIDDEN);
            WiFi.scanNetworks(true);
            lv_timer_create([](lv_timer_t *timer) {
                LvglEventManager::refreshImageAngle = (LvglEventManager::refreshImageAngle + 150) % 3600;
                lv_obj_set_style_transform_rotation(lv_ui.menu_network_page_wifi_list_loading_image,
                                                    LvglEventManager::refreshImageAngle, 0);
                const int16_t result = WiFi.scanComplete();
                if (result >= 0) {
                    lv_obj_add_flag(lv_ui.menu_network_page_wifi_list_loading_image, LV_OBJ_FLAG_HIDDEN);
                    for (int i = 0; i < result; i++) {
                        const auto button = lv_list_add_button(lv_ui.menu_network_page_wifi_list, LV_CUSTOM_SYMBOL_WIFI,
                                                               WiFi.SSID(i).c_str());
                        lv_obj_add_event_cb(button, network_setting_wifi_item_click_cb, LV_EVENT_CLICKED, button);
                    }
                    lv_timer_delete(timer);
                    LvglEventManager::isRefreshingWiFi = false;
                    LvglEventManager::refreshImageAngle = 0;
                }
            }, 500, nullptr);
            LvglEventManager::isRefreshingWiFi = true;
        }
    }
}

void LvglEventManager::registerLvglEventCallback() {
    lv_obj_add_event_cb(lv_ui.menu_network_page_refresh_button,
                        network_setting_refresh_wifi_button_cb, LV_EVENT_CLICKED, nullptr);
    lv_obj_add_event_cb(lv_ui.confirm_button,
                        network_setting_password_dialog_confirm_button_cb, LV_EVENT_CLICKED, nullptr);
    lv_obj_add_event_cb(lv_ui.cancel_button,
                        hide_wifi_password_dialog_cb, LV_EVENT_CLICKED, nullptr);

    lv_obj_add_event_cb(lv_ui.menu_network_page_keyboard_mask, hide_wifi_password_dialog_cb, LV_EVENT_CLICKED, NULL);
}
