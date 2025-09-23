#include "lvgl.h"
#include "gui.h"
#include "screen_home.h"
#include "screen_settings.h"


void color_grad_anim_cb(void *obj, int32_t value) {
    lv_obj_set_style_bg_opa(obj, value, 0);
    lv_obj_set_style_bg_grad_opa(obj, value, 0);
}

void setup_ui(lv_screens_info *ui) {
    ui->status_bar = lv_obj_create(lv_layer_top());
    lv_obj_set_size(ui->status_bar, LV_HOR_RES, STATUS_BAR_HEIGHT);
    lv_obj_remove_flag(ui->status_bar, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(ui->status_bar, 0, 0);
    lv_obj_set_style_bg_color(lv_ui.status_bar, lv_color_hex(0xff3300), 0);
    lv_obj_set_style_bg_grad_color(lv_ui.status_bar, lv_color_hex(0x0073e6), 0);
    lv_obj_set_style_bg_grad_dir(lv_ui.status_bar, LV_GRAD_DIR_HOR, 0);
    lv_obj_set_style_text_font(ui->status_bar, &SourceHanSerifCN_15, 0);
    lv_obj_set_style_radius(ui->status_bar, 0, 0);
    lv_obj_set_style_text_color(ui->status_bar, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_layout(ui->status_bar, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(ui->status_bar, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui->status_bar, LV_FLEX_ALIGN_SPACE_BETWEEN,
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // 状态栏颜色动画
    lv_anim_init(&lv_ui.status_bar_anim);
    lv_anim_set_var(&lv_ui.status_bar_anim, lv_ui.status_bar);
    lv_anim_set_values(&lv_ui.status_bar_anim, 150, 255);
    lv_anim_set_duration(&lv_ui.status_bar_anim, 1000);
    lv_anim_set_reverse_duration(&lv_ui.status_bar_anim, 1000);
    lv_anim_set_exec_cb(&lv_ui.status_bar_anim, color_grad_anim_cb);
    lv_anim_set_repeat_count(&lv_ui.status_bar_anim, -1);

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
