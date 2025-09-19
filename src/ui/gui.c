#include "gui.h"
#include "screen_home.h"
#include "lvgl.h"
#include "screen_settings.h"

void setup_ui(lv_screens_info *ui) {
    ui->status_bar = lv_obj_create(lv_layer_top());
    lv_obj_set_size(ui->status_bar, LV_HOR_RES, STATUS_BAR_HEIGHT);
    lv_obj_set_scrollbar_mode(ui->status_bar, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_width(ui->status_bar, 0, 0);
    lv_obj_set_style_bg_color(ui->status_bar, lv_color_hex(0x666666), 0);
    lv_obj_set_style_text_font(ui->status_bar, &AlibabaPuHuiTi_Regular_16, 0);
    lv_obj_set_style_radius(ui->status_bar, 0, 0);
    lv_obj_set_style_text_color(ui->status_bar, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_layout(ui->status_bar, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(ui->status_bar, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui->status_bar, LV_FLEX_ALIGN_SPACE_BETWEEN,
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    ui->status_bar_time_label = lv_label_create(ui->status_bar);
    lv_label_set_text(ui->status_bar_time_label, "00:00");

    ui->status_bar_state_label = lv_label_create(ui->status_bar);
    lv_label_set_text(ui->status_bar_state_label, "正在聆听...");

    ui->status_bar_wifi_image = lv_image_create(ui->status_bar);
    lv_obj_set_size(ui->status_bar_wifi_image, 30, 30);
    lv_image_set_src(ui->status_bar_wifi_image, LV_CUSTOM_SYMBOL_WIFI);

    setup_screen_home(ui);
    setup_screen_settings(ui);
}
