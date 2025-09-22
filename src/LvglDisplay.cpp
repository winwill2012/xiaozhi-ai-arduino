#include "LvglDisplay.h"
#include <Arduino.h>
#include "FT6336.h"
#include "LvglEventManager.h"

extern "C" {
#include "ui/gui.h"
}

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

    xTaskCreate(lvgl_loop, "lvgl-loop", 8192, nullptr, 1, nullptr);
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
