#include "screen_settings_reset.h"
#include "lvgl.h"

void reset_button_click_event_callback(lv_event_t *event) {
    if (lv_event_get_code(event) == LV_EVENT_CLICKED) {
        lv_obj_remove_flag(lv_ui.menu_page_reset_msgbox, LV_OBJ_FLAG_HIDDEN);
    }
}

void cancel_button_click_event_callback(lv_event_t *event) {
    if (lv_event_get_code(event) == LV_EVENT_CLICKED) {
        lv_obj_add_flag(lv_ui.menu_page_reset_msgbox, LV_OBJ_FLAG_HIDDEN);
    }
}

void setup_settings_reset(lv_screens_info *ui) {
    ui->menu_page_reset = lv_menu_page_create(ui->menu, "重置");
    lv_obj_set_layout(ui->menu_page_reset, LV_LAYOUT_NONE);
    ui->menu_page_reset_button = lv_button_create(ui->menu_page_reset);
    lv_obj_set_size(ui->menu_page_reset_button, lv_pct(60), 30);
    lv_obj_center(ui->menu_page_reset_button);
    lv_obj_set_style_bg_color(ui->menu_page_reset_button, lv_color_hex(0xFF0000), 0);
    ui->menu_page_reset_button_label = lv_label_create(ui->menu_page_reset_button);
    lv_label_set_text(ui->menu_page_reset_button_label, "恢复出厂设置");
    lv_obj_center(ui->menu_page_reset_button_label);

    ui->menu_page_reset_msgbox = lv_msgbox_create(ui->menu_page_reset);
    lv_obj_set_style_pad_all(ui->menu_page_reset_msgbox, 5, 0);
    lv_obj_set_size(ui->menu_page_reset_msgbox, lv_pct(90), LV_SIZE_CONTENT);
    lv_msgbox_add_title(ui->menu_page_reset_msgbox, "确定恢复出厂设置吗？");
    lv_obj_set_style_text_align(ui->menu_page_reset_msgbox, LV_TEXT_ALIGN_CENTER, 0);
    ui->menu_page_reset_msgbox_confirm_button = lv_msgbox_add_footer_button(ui->menu_page_reset_msgbox, "重置");
    lv_obj_set_style_bg_color(ui->menu_page_reset_msgbox_confirm_button, lv_color_hex(0xFF0000), 0);
    ui->menu_page_reset_msgbox_cancel_button = lv_msgbox_add_footer_button(ui->menu_page_reset_msgbox, "取消");
    lv_obj_center(ui->menu_page_reset_msgbox);
    lv_obj_add_flag(ui->menu_page_reset_msgbox, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_event_cb(ui->menu_page_reset_button, reset_button_click_event_callback, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui->menu_page_reset_msgbox_cancel_button, cancel_button_click_event_callback, LV_EVENT_CLICKED,
                        NULL);
}
