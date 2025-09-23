#include "screen_settings_network.h"
#include "lvgl.h"
#include "gui.h"

void anim_exec_cb(void *obj, int32_t x) {
    lv_obj_set_style_transform_rotation(obj, x, 0);
}

static int32_t refresh_image_angle = 0;
static bool loading_wifi = false;

void wifi_item_event_callback(lv_event_t *event) {
    if (lv_event_get_code(event) == LV_EVENT_CLICKED) {
        lv_obj_t *clicked_item = lv_event_get_target(event);
        lv_ui.clicked_wifi_name = lv_list_get_button_text(lv_ui.menu_network_page_wifi_list, clicked_item);
        char title[100];
        lv_snprintf(title, sizeof(title), "请输入%s密码", lv_ui.clicked_wifi_name);
        lv_label_set_text(lv_ui.menu_network_page_password_tips_label, title);
        lv_textarea_set_text(lv_ui.menu_network_page_password_textarea, "");
        lv_obj_remove_flag(lv_ui.menu_network_page_keyboard_mask, LV_OBJ_FLAG_HIDDEN);
    }
}

void load_wifi_info_timer_cb(lv_timer_t *timer) {
    refresh_image_angle = (refresh_image_angle + 100) % 3600;
    lv_obj_set_style_transform_rotation(lv_ui.menu_network_page_wifi_list_loading_image, refresh_image_angle, 0);
    if (refresh_image_angle == 1000) {
        lv_obj_add_flag(lv_ui.menu_network_page_wifi_list_loading_image, LV_OBJ_FLAG_HIDDEN);
        refresh_image_angle = 0;
        loading_wifi = false;
        for (int i = 0; i < 10; i++) {
            char wifi_name[50];
            lv_snprintf(wifi_name, sizeof(wifi_name), "wifi%d", i);
            lv_obj_t *item = lv_list_add_button(lv_ui.menu_network_page_wifi_list, LV_CUSTOM_SYMBOL_WIFI, wifi_name);
            lv_obj_add_event_cb(item, wifi_item_event_callback, LV_EVENT_CLICKED, NULL);
        }
        lv_timer_delete(timer);
    }
}

void hide_mast_event_callback(lv_event_t *event) {
    if (lv_event_get_code(event) == LV_EVENT_CLICKED) {
        lv_obj_add_flag(lv_ui.menu_network_page_keyboard_mask, LV_OBJ_FLAG_HIDDEN);
    }
}


void setup_network_setting(lv_screens_info *ui) {
    ui->menu_network_page = lv_menu_page_create(ui->menu, "网络设置");

    ui->menu_network_page_cont1 = lv_menu_cont_create(ui->menu_network_page);
    ui->menu_network_page_cont2 = lv_menu_cont_create(ui->menu_network_page);
    lv_obj_set_layout(ui->menu_network_page_cont2, LV_LAYOUT_NONE);
    lv_obj_set_flex_grow(ui->menu_network_page_cont2, 1);

    ui->menu_network_page_tips_label = lv_label_create(ui->menu_network_page_cont1);
    lv_label_set_text(ui->menu_network_page_tips_label, "连接附近的WLAN");

    ui->menu_network_page_refresh_button = lv_button_create(ui->menu_network_page_cont1);
    lv_obj_set_style_margin_left(ui->menu_network_page_refresh_button, 10, 0);
    lv_obj_set_style_pad_all(ui->menu_network_page_refresh_button, 5, 0);
    ui->menu_network_page_refresh_button_label = lv_label_create(ui->menu_network_page_refresh_button);
    lv_label_set_text(ui->menu_network_page_refresh_button_label, LV_CUSTOM_SYMBOL_RESET" 刷新");
    lv_obj_center(ui->menu_network_page_refresh_button_label);

    ui->menu_network_page_wifi_list = lv_list_create(ui->menu_network_page_cont2);
    lv_obj_set_size(ui->menu_network_page_wifi_list, lv_pct(100), lv_pct(100));

    ui->menu_network_page_wifi_list_loading_image = lv_image_create(ui->menu_network_page_cont2);
    lv_obj_set_size(ui->menu_network_page_wifi_list_loading_image, 50, 50);
    lv_image_set_src(ui->menu_network_page_wifi_list_loading_image, LV_CUSTOM_SYMBOL_LOADING);
    lv_obj_set_style_transform_pivot_x(ui->menu_network_page_wifi_list_loading_image, 25, 0);
    lv_obj_set_style_transform_pivot_y(ui->menu_network_page_wifi_list_loading_image, 25, 0);
    lv_obj_center(ui->menu_network_page_wifi_list_loading_image);
    lv_obj_add_flag(ui->menu_network_page_wifi_list_loading_image, LV_OBJ_FLAG_HIDDEN);

    ui->menu_network_page_keyboard_mask = lv_obj_create(lv_layer_top());
    lv_obj_remove_style_all(ui->menu_network_page_keyboard_mask);
    lv_obj_set_size(ui->menu_network_page_keyboard_mask, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_pos(ui->menu_network_page_keyboard_mask, 0, 0);
    lv_obj_set_style_bg_opa(ui->menu_network_page_keyboard_mask, LV_OPA_0, 0);
    lv_obj_set_style_pad_all(ui->menu_network_page_keyboard_mask, 0, 0);
    lv_obj_set_layout(ui->menu_network_page_keyboard_mask, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(ui->menu_network_page_keyboard_mask, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui->menu_network_page_keyboard_mask, LV_FLEX_ALIGN_END,
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_END);
    lv_obj_set_style_text_font(ui->menu_network_page_keyboard_mask, &SourceHanSerifCN_15, 0);
    lv_obj_add_flag(ui->menu_network_page_keyboard_mask, LV_OBJ_FLAG_HIDDEN); // 隐藏遮罩层

    ui->menu_network_page_password_dialog = lv_obj_create(ui->menu_network_page_keyboard_mask);
    lv_obj_set_size(ui->menu_network_page_password_dialog, lv_pct(95), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_color(ui->menu_network_page_password_dialog, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_border_width(ui->menu_network_page_password_dialog, 1, 0);
    lv_obj_set_style_border_color(ui->menu_network_page_password_dialog, lv_color_hex(0x85adad), 0);
    lv_obj_set_style_pad_ver(ui->menu_network_page_password_dialog, 10, 0);
    lv_obj_set_flex_flow(ui->menu_network_page_password_dialog, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui->menu_network_page_password_dialog, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);

    ui->menu_network_page_password_tips_label = lv_label_create(ui->menu_network_page_password_dialog);

    ui->menu_network_page_password_textarea = lv_textarea_create(ui->menu_network_page_password_dialog);
    lv_obj_set_size(ui->menu_network_page_password_textarea, lv_pct(95), 35);
    lv_obj_set_style_border_width(ui->menu_network_page_password_textarea, 1, 0);
    lv_obj_set_style_border_color(ui->menu_network_page_password_textarea, lv_color_hex(0x00ace6), 0);
    lv_textarea_set_one_line(ui->menu_network_page_password_textarea, true);
    lv_textarea_set_placeholder_text(ui->menu_network_page_password_textarea, "请输入WiFi密码");
    lv_obj_remove_flag(ui->menu_network_page_password_textarea, LV_OBJ_FLAG_SCROLLABLE);

    ui->button_cont = lv_obj_create(ui->menu_network_page_password_dialog);
    lv_obj_remove_style_all(ui->button_cont);
    lv_obj_set_size(ui->button_cont, lv_pct(75), 30);
    lv_obj_set_layout(ui->button_cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(ui->button_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui->button_cont, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    ui->confirm_button = lv_button_create(ui->button_cont);
    ui->cancel_button = lv_button_create(ui->button_cont);
    lv_obj_set_size(ui->confirm_button, 60, 30);
    lv_obj_set_size(ui->cancel_button, 60, 30);
    lv_obj_set_style_bg_color(ui->cancel_button, lv_color_hex(0xFF0000), 0);
    ui->confirm_button_label = lv_label_create(ui->confirm_button);
    ui->cancel_button_label = lv_label_create(ui->cancel_button);
    lv_label_set_text(ui->confirm_button_label, "确定");
    lv_label_set_text(ui->cancel_button_label, "取消");
    lv_obj_center(ui->confirm_button_label);
    lv_obj_center(ui->cancel_button_label);

    ui->menu_network_page_keyboard = lv_keyboard_create(ui->menu_network_page_keyboard_mask);
    lv_obj_align(ui->menu_network_page_keyboard, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_keyboard_set_textarea(ui->menu_network_page_keyboard, ui->menu_network_page_password_textarea);
}
