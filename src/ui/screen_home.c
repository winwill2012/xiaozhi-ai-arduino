#include "screen_home.h"
#include "lvgl.h"
#include "gui.h"
#include "screen_settings.h"

void screen_home_event_callback(lv_event_t *event) {
    const lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_GESTURE) {
        const lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        if (dir == LV_DIR_LEFT) {
            lv_screens_info *ui = (lv_screens_info *) lv_event_get_user_data(event);
            lv_indev_wait_release(lv_indev_active());
            setup_screen_settings(ui);
            lv_screen_load(ui->screen_settings);
        }
    }
}

void setup_screen_home(lv_screens_info *ui) {
    ui->screen_home = lv_obj_create(NULL);
    lv_obj_set_style_pad_all(ui->screen_home, 0, 0);
    lv_obj_set_style_text_font(ui->screen_home, &AlibabaPuHuiTi_Regular_16, 0);
    ui->screen_home_speak_button = lv_button_create(ui->screen_home);
    lv_obj_set_size(ui->screen_home_speak_button, lv_pct(95), 25);
    lv_obj_set_style_bg_color(ui->screen_home_speak_button, lv_color_hex(0x00b386), 0);
    lv_obj_set_style_radius(ui->screen_home_speak_button, 3, 0);
    ui->screen_home_speak_button_label = lv_label_create(ui->screen_home_speak_button);
    lv_obj_center(ui->screen_home_speak_button_label);
    lv_label_set_text(ui->screen_home_speak_button_label, "点击说话");
    lv_obj_align(ui->screen_home_speak_button, LV_ALIGN_BOTTOM_MID, 0, -5);

    lv_screen_load(ui->screen_home);

    lv_obj_add_event_cb(ui->screen_home, screen_home_event_callback, LV_EVENT_ALL, ui);
}
