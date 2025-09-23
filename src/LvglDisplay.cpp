#include "LvglDisplay.h"
#include <Arduino.h>
#include "FT6336.h"
#include "LvglEventManager.h"
#include "Settings.h"

extern "C" {
#include "ui/gui.h"
}

#define TFT_BL_PIN 8 // 屏幕背光引脚
#define TOUCH_SDA 10
#define TOUCH_SCL 13
#define TOUCH_INT 12
#define TOUCH_RST 9
#define TFT_HOR_RES   240
#define TFT_VER_RES   320
#define TFT_ROTATION  LV_DISPLAY_ROTATION_0

#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

FT6336 ft(TOUCH_SDA, TOUCH_SCL, TOUCH_INT, TOUCH_RST, TFT_HOR_RES, TFT_VER_RES);

lv_screens_info lv_ui;
lv_obj_t *LvglDisplay::last_message;
int LvglDisplay::current_message_number = 0;
SemaphoreHandle_t LvglDisplay::lvglUpdateLock = xSemaphoreCreateRecursiveMutex();

void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data) {
    ft.read();
    if (!ft.isTouched) {
        data->state = LV_INDEV_STATE_RELEASED;
    } else {
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = ft.points[0].x;
        data->point.y = ft.points[0].y;
    }
}

static uint32_t my_tick(void) {
    return millis();
}

void lvgl_loop(void *ptr) {
    while (true) {
        if (xSemaphoreTakeRecursive(LvglDisplay::lvglUpdateLock, portMAX_DELAY) == pdTRUE) {
            lv_timer_handler();
            xSemaphoreGiveRecursive(LvglDisplay::lvglUpdateLock);
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

void LvglDisplay::begin() {
    // 调节屏幕亮度
    pinMode(TFT_BL_PIN, OUTPUT);
    analogWrite(TFT_BL_PIN, static_cast<int>(Settings::getScreenBrightness() * 2.55));

    lv_init();

    ft.begin();
    lv_tick_set_cb(my_tick);

    lv_display_t *disp;
    disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, sizeof(draw_buf));
    lv_display_set_rotation(disp, TFT_ROTATION);

    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); /*Touchpad should have POINTER type*/
    lv_indev_set_read_cb(indev, my_touchpad_read);

    // 创建一个对象
    setup_ui(&lv_ui);

    LvglEventManager::registerLvglEventCallback();

    loadSettings(); // 从配置信息中加载相关配置
    forbidSpeak(true);
    xTaskCreate(lvgl_loop, "lvgl-loop", 8192, nullptr, 1, nullptr);
}

void LvglDisplay::loadSettings() {
    const std::map<std::string, std::string> &voiceMap = Settings::getVoiceMap();
    const std::map<std::string, std::string> &personaMap = Settings::getPersonaMap();
    std::string voiceList;
    std::string personaList;
    uint16_t selectedVoiceIndex = 0;
    uint16_t selectedPersonaIndex = 0;
    uint16_t i = 0, j = 0;
    for (const auto &item: voiceMap) {
        voiceList += (item.first + "\n");
        if (Settings::getCurrentVoice().compareTo(item.first.c_str()) == 0) {
            selectedVoiceIndex = i;
        }
        i++;
    }
    voiceList.pop_back();
    for (const auto &item: personaMap) {
        personaList += (item.first + "\n");
        if (Settings::getCurrentPersona().compareTo(item.first.c_str()) == 0) {
            selectedPersonaIndex = j;
        }
        j++;
    }
    personaList.pop_back();
    lv_dropdown_set_options(lv_ui.menu_llm_page_voice_dropdown, voiceList.c_str());
    lv_dropdown_set_selected(lv_ui.menu_llm_page_voice_dropdown, selectedVoiceIndex);

    lv_dropdown_set_options(lv_ui.menu_llm_page_persona_dropdown, personaList.c_str());
    lv_dropdown_set_selected(lv_ui.menu_llm_page_persona_dropdown, selectedPersonaIndex);

    lv_slider_set_value(lv_ui.menu_llm_page_speed_slider,
                        static_cast<int32_t>(Settings::getCurrentSpeakSpeedRatio() * 10),
                        LV_ANIM_OFF);
    lv_slider_set_value(lv_ui.control_bar_volume_slider,
                        static_cast<int32_t>(Settings::getCurrentSpeakVolumeRatio() * 100),
                        LV_ANIM_OFF);
    lv_slider_set_value(lv_ui.control_bar_brightness_slider,
                        Settings::getScreenBrightness(),
                        LV_ANIM_ON);
}

void LvglDisplay::registerLvglEventCallback() {
    lv_obj_add_event_cb(lv_ui.menu_network_page_refresh_button, nullptr, LV_EVENT_CLICKED, nullptr);
}

void LvglDisplay::updateTime(const std::string &time) {
    if (xSemaphoreTakeRecursive(lvglUpdateLock, portMAX_DELAY) == pdTRUE) {
        lv_label_set_text(lv_ui.status_bar_time_label, time.c_str());
        xSemaphoreGiveRecursive(lvglUpdateLock);
    }
}

void LvglDisplay::updateState(const std::string &state) {
    if (xSemaphoreTakeRecursive(lvglUpdateLock, portMAX_DELAY) == pdTRUE) {
        lv_label_set_text(lv_ui.status_bar_state_label, state.c_str());
        xSemaphoreGiveRecursive(lvglUpdateLock);
    }
}

void LvglDisplay::updateChatText(const bool newLine, const std::string &text) {
    if (xSemaphoreTakeRecursive(lvglUpdateLock, portMAX_DELAY) == pdTRUE) {
        // 如果不是新消息，则更新之前的数据即可
        if (!newLine && last_message != nullptr) {
            lv_label_set_text(last_message, text.c_str());
            lv_obj_scroll_to_y(lv_ui.screen_home_message_list, LV_COORD_MAX, LV_ANIM_ON);
            xSemaphoreGiveRecursive(lvglUpdateLock);
            return;
        }
        // 是一条新消息，将新消息插入到消息列表中
        last_message = lv_list_add_text(lv_ui.screen_home_message_list, text.c_str());
        lv_obj_set_style_bg_color(last_message, lv_color_hex(0xc2f0f0), 0);
        lv_label_set_long_mode(last_message, LV_LABEL_LONG_WRAP);
        current_message_number++;
        lv_obj_set_style_text_align(last_message, LV_TEXT_ALIGN_LEFT, 0);
        lv_obj_set_style_pad_top(last_message, 20, 0);
        lv_obj_set_style_bg_opa(last_message, 0, 0);
        lv_obj_set_style_text_color(last_message, lv_color_make(0, 0, 0), 0);
        if (current_message_number > 20) {
            lv_obj_t *first_message = lv_obj_get_child(lv_ui.screen_home_message_list, 0);
            if (first_message != nullptr) {
                lv_obj_delete(first_message);
                current_message_number--;
            }
        }
        lv_obj_scroll_to_y(lv_ui.screen_home_message_list, LV_COORD_MAX, LV_ANIM_ON);
        xSemaphoreGiveRecursive(lvglUpdateLock);
    }
}

void LvglDisplay::forbidSpeak(const bool forbid) {
    if (forbid) {
        lv_obj_remove_flag(lv_ui.screen_home_speak_button, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_bg_color(lv_ui.screen_home_speak_button, lv_color_hex(0xffad33), 0);
        lv_label_set_text(lv_ui.screen_home_speak_button_label, LV_CUSTOM_SYMBOL_MIC" 请先双击状态栏联网");
    } else {
        lv_obj_add_flag(lv_ui.screen_home_speak_button, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_bg_color(lv_ui.screen_home_speak_button, lv_color_hex(0x00b386), 0);
        lv_label_set_text(lv_ui.screen_home_speak_button_label, LV_CUSTOM_SYMBOL_MIC" 按住说话");
    }
}