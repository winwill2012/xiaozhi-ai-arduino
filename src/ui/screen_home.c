#include "screen_home.h"
#include "lvgl.h"
#include "gui.h"

void anim_y_exe_cb(void *obj, int32_t y) {
    lv_obj_set_y(obj, y);
}

void hide_control_bar(uint32_t duration) {
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, lv_ui.control_bar);
    lv_anim_set_values(&anim, 0, -lv_obj_get_height(lv_ui.control_bar));
    lv_anim_set_duration(&anim, duration);
    lv_anim_set_exec_cb(&anim, anim_y_exe_cb);
    lv_anim_start(&anim);
    lv_ui.control_bar_active = false;
}

void show_control_bar(uint32_t duration) {
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, lv_ui.control_bar);
    lv_anim_set_values(&anim, -lv_obj_get_height(lv_ui.control_bar), 0);
    lv_anim_set_duration(&anim, duration);
    lv_anim_set_exec_cb(&anim, anim_y_exe_cb);
    lv_anim_start(&anim);
    lv_ui.control_bar_active = true;
}

void more_settings_click_event_callback(lv_event_t *event) {
    if (lv_event_get_code(event) == LV_EVENT_CLICKED) {
        hide_control_bar(100);
        lv_screen_load_anim(lv_ui.screen_settings, LV_SCR_LOAD_ANIM_OVER_LEFT, 200, 0, false);
    }
}

void status_bar_event_callback(lv_event_t *event) {
    if (lv_event_get_code(event) == LV_EVENT_DOUBLE_CLICKED) {
        if (!lv_ui.control_bar_active) {
            show_control_bar(300);
        } else {
            hide_control_bar(300);
        }
    }
}

void screen_home_event_callback(lv_event_t *event) {
    const lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_GESTURE) {
        const lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        if (dir == LV_DIR_BOTTOM && !lv_ui.control_bar_active) {
            lv_indev_wait_release(lv_indev_active());

            lv_anim_t anim;
            lv_anim_init(&anim);
            lv_anim_set_var(&anim, lv_ui.control_bar);
            lv_anim_set_values(&anim, -lv_obj_get_height(lv_ui.control_bar), 0);
            lv_anim_set_duration(&anim, 500);
            lv_anim_set_exec_cb(&anim, anim_y_exe_cb);
            lv_anim_start(&anim);
            lv_ui.control_bar_active = true;
        } else if (dir == LV_DIR_TOP && lv_ui.control_bar_active) {
            lv_indev_wait_release(lv_indev_active());

            lv_anim_t anim;
            lv_anim_init(&anim);
            lv_anim_set_var(&anim, lv_ui.control_bar);
            lv_anim_set_values(&anim, 0, -lv_obj_get_height(lv_ui.control_bar));
            lv_anim_set_duration(&anim, 500);
            lv_anim_set_exec_cb(&anim, anim_y_exe_cb);
            lv_anim_start(&anim);
            lv_ui.control_bar_active = false;
        }
    }
}

// 初始化顶部控制栏
void setup_control_bar() {
    lv_ui.control_bar = lv_obj_create(lv_ui.screen_home);
    lv_obj_set_style_pad_all(lv_ui.control_bar, 0, 0);
    lv_obj_set_style_bg_color(lv_ui.control_bar, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_bg_opa(lv_ui.control_bar, 0, 0);
    lv_obj_set_style_text_font(lv_ui.control_bar, &AlibabaPuHuiTi_Regular_15, 0);
    lv_obj_set_style_border_width(lv_ui.control_bar, 0, 0);
    lv_obj_set_style_radius(lv_ui.control_bar, 0, 0);

    lv_ui.control_bar_cont = lv_obj_create(lv_ui.control_bar);
    lv_obj_set_size(lv_ui.control_bar_cont, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_align(lv_ui.control_bar_cont, LV_ALIGN_TOP_MID, 0, STATUS_BAR_HEIGHT);
    lv_obj_set_style_pad_all(lv_ui.control_bar_cont, 10, 0);
    lv_obj_set_style_bg_color(lv_ui.control_bar_cont, lv_color_hex(0xe5f9ff), 0);
    lv_obj_remove_flag(lv_ui.control_bar_cont, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_layout(lv_ui.control_bar_cont, LV_LAYOUT_GRID);
    static const int32_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(5), LV_GRID_TEMPLATE_LAST};
    static const int32_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(lv_ui.control_bar_cont, col_dsc, row_dsc);
    lv_obj_set_grid_align(lv_ui.control_bar_cont, LV_GRID_ALIGN_CENTER, LV_GRID_ALIGN_CENTER);

    lv_ui.control_bar_volume_image = lv_image_create(lv_ui.control_bar_cont);
    lv_obj_set_size(lv_ui.control_bar_volume_image, 30, 30);
    lv_image_set_src(lv_ui.control_bar_volume_image, LV_CUSTOM_SYMBOL_VOLUME);
    lv_obj_set_grid_cell(lv_ui.control_bar_volume_image, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);

    lv_ui.control_bar_volume_slider = lv_slider_create(lv_ui.control_bar_cont);
    lv_obj_set_height(lv_ui.control_bar_volume_slider, 10);
    lv_slider_set_range(lv_ui.control_bar_volume_slider, 0, 100);
    lv_slider_set_value(lv_ui.control_bar_volume_slider, 80, LV_ANIM_ON);
    lv_obj_set_grid_cell(lv_ui.control_bar_volume_slider, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);

    lv_ui.control_bar_brightness_image = lv_image_create(lv_ui.control_bar_cont);
    lv_obj_set_size(lv_ui.control_bar_brightness_image, 30, 30);
    lv_image_set_src(lv_ui.control_bar_brightness_image, LV_CUSTOM_SYMBOL_BRIGHTNESS);
    lv_obj_set_grid_cell(lv_ui.control_bar_brightness_image, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);

    lv_ui.control_bar_brightness_slider = lv_slider_create(lv_ui.control_bar_cont);
    lv_obj_set_height(lv_ui.control_bar_brightness_slider, 10);
    lv_slider_set_range(lv_ui.control_bar_brightness_slider, 30, 100);
    lv_slider_set_value(lv_ui.control_bar_brightness_slider, 80, LV_ANIM_ON);
    lv_obj_set_grid_cell(lv_ui.control_bar_brightness_slider, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_CENTER, 1, 1);

    lv_ui.control_bar_more_settings_button = lv_button_create(lv_ui.control_bar_cont);
    lv_obj_set_size(lv_ui.control_bar_more_settings_button, LV_SIZE_CONTENT, 30);
    lv_ui.control_bar_more_settings_button_label = lv_label_create(lv_ui.control_bar_more_settings_button);
    lv_obj_center(lv_ui.control_bar_more_settings_button_label);
    lv_label_set_text(lv_ui.control_bar_more_settings_button_label, LV_CUSTOM_SYMBOL_SETTINGS" 设置");
    lv_obj_set_grid_cell(lv_ui.control_bar_more_settings_button, LV_GRID_ALIGN_END, 1, 1, LV_GRID_ALIGN_CENTER, 2, 1);

    lv_obj_set_size(lv_ui.control_bar, LV_HOR_RES, LV_SIZE_CONTENT);
    lv_obj_align_to(lv_ui.control_bar, lv_ui.screen_home, LV_ALIGN_OUT_TOP_MID, 0, 0);
    lv_ui.control_bar_active = false;
}

void setup_screen_home() {
    if (lv_ui.screen_home != NULL) {
        return;
    }
    lv_ui.screen_home = lv_obj_create(NULL);
    lv_obj_set_style_text_font(lv_ui.screen_home, &AlibabaPuHuiTi_Regular_15, 0);

    lv_ui.screen_home_message_list = lv_list_create(lv_ui.screen_home);
    lv_obj_set_size(lv_ui.screen_home_message_list, lv_pct(95),
                    LV_VER_RES - STATUS_BAR_HEIGHT - SPEAK_BUTTON_HEIGHT - 15);
    lv_obj_align(lv_ui.screen_home_message_list, LV_ALIGN_TOP_MID, 0, STATUS_BAR_HEIGHT + 5);
    lv_obj_set_style_border_width(lv_ui.screen_home_message_list, 0, 0);

    lv_ui.screen_home_speak_button = lv_button_create(lv_ui.screen_home);
    lv_obj_set_size(lv_ui.screen_home_speak_button, lv_pct(95), SPEAK_BUTTON_HEIGHT);
    lv_obj_set_style_bg_color(lv_ui.screen_home_speak_button, lv_color_hex(0x00b386), 0);
    lv_obj_set_style_radius(lv_ui.screen_home_speak_button, 5, 0);
    lv_ui.screen_home_speak_button_label = lv_label_create(lv_ui.screen_home_speak_button);
    lv_obj_center(lv_ui.screen_home_speak_button_label);
    lv_label_set_text(lv_ui.screen_home_speak_button_label, LV_CUSTOM_SYMBOL_MIC" 按住说话");
    lv_obj_align(lv_ui.screen_home_speak_button, LV_ALIGN_BOTTOM_MID, 0, -5);

    setup_control_bar();

    lv_screen_load(lv_ui.screen_home);
    lv_obj_add_event_cb(lv_ui.status_bar, status_bar_event_callback, LV_EVENT_DOUBLE_CLICKED, NULL);
    lv_obj_add_event_cb(lv_ui.screen_home, screen_home_event_callback, LV_EVENT_GESTURE, NULL);
    lv_obj_add_event_cb(lv_ui.control_bar, screen_home_event_callback, LV_EVENT_GESTURE, NULL);
    lv_obj_add_event_cb(lv_ui.control_bar_more_settings_button, more_settings_click_event_callback,
                        LV_EVENT_CLICKED, NULL);
}
