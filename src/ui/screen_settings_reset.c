#include "screen_settings_reset.h"
#include "lvgl.h"

void setup_settings_reset(lv_screens_info *ui) {
    ui->menu_page_reset = lv_menu_page_create(ui->menu, "重置");

    ui->menu_page_reset_button = lv_button_create(ui->menu_page_reset);
    lv_obj_set_size(ui->menu_page_reset_button, lv_pct(60), 30);
    lv_obj_set_style_bg_color(ui->menu_page_reset_button, lv_color_hex(0xAA0000), 0);
    ui->menu_page_reset_button_label = lv_label_create(ui->menu_page_reset_button);
    lv_label_set_text(ui->menu_page_reset_button_label, "恢复出厂设置");
    lv_obj_center(ui->menu_page_reset_button_label);
    lv_obj_set_style_margin_top(ui->menu_page_reset_button, 20, 0);
}
