#include "screen_settings_about.h"
#include "lvgl.h"

void setup_settings_about(lv_screens_info *ui) {
    ui->menu_page_about = lv_menu_page_create(ui->menu, "关于我们");

    lv_obj_set_flex_align(ui->menu_page_about, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER);
    ui->menu_page_about_label1 = lv_label_create(ui->menu_page_about);
    ui->menu_page_about_label2 = lv_label_create(ui->menu_page_about);
    lv_label_set_text(ui->menu_page_about_label1, "Arduino 大模型聊天机器人V1.0");
    lv_label_set_text(ui->menu_page_about_label2, "微联编程出品");
    lv_obj_set_style_margin_ver(ui->menu_page_about_label1, 10, 0);
    lv_obj_set_style_margin_ver(ui->menu_page_about_label2, 10, 0);
}
