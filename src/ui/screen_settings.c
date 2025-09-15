#include "screen_settings.h"
#include "src/core/lv_obj.h"
#include "src/widgets/label/lv_label.h"
#include "src/widgets/menu/lv_menu.h"
#include "gui.h"

void menu_event_callback(lv_event_t *event) {
    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_CLICKED) {
        const lv_screens_info *ui = (lv_screens_info *) lv_event_get_user_data(event);
        lv_obj_t *button = lv_event_get_target(event);
        if (lv_menu_back_button_is_root(ui->menu, button)) {
            lv_screen_load(ui->screen_home);
        }
    }
}

void setup_screen_settings(lv_screens_info *ui) {
    ui->screen_settings = lv_obj_create(NULL);
    lv_obj_set_style_pad_all(ui->screen_settings, 0, 0);
    lv_obj_set_style_text_font(ui->screen_settings, &AlibabaPuHuiTi_Regular_16, 0);
    ui->menu = lv_menu_create(ui->screen_settings);
    lv_obj_set_size(ui->menu, LV_HOR_RES, LV_VER_RES - 30);
    lv_obj_align(ui->menu, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(ui->menu, lv_color_hex(0xDDDDDD), 0);
    lv_menu_set_mode_header(ui->menu, LV_MENU_HEADER_TOP_FIXED);
    lv_menu_set_mode_root_back_button(ui->menu, LV_MENU_ROOT_BACK_BUTTON_ENABLED);

    // 创建设置主界面
    ui->menu_main_page = lv_menu_page_create(ui->menu, "设置");
    ui->menu_main_page_seperator1 = lv_menu_separator_create(ui->menu_main_page);

    static lv_style_t menu_item_style;
    lv_style_init(&menu_item_style);
    lv_style_set_margin_ver(&menu_item_style, 5);
    lv_style_set_pad_all(&menu_item_style, 5);

    ui->menu_main_page_section_main = lv_menu_section_create(ui->menu_main_page);
    lv_obj_set_style_pad_hor(ui->menu_main_page_section_main, 5, 0);

    ui->menu_main_page_label_network = lv_label_create(ui->menu_main_page_section_main);
    lv_label_set_text(ui->menu_main_page_label_network, LV_CUSTOM_SYMBOL_WIFI " 网络");
    lv_obj_add_style(ui->menu_main_page_label_network, &menu_item_style, 0);

    ui->menu_main_page_label_volume = lv_label_create(ui->menu_main_page_section_main);
    lv_label_set_text(ui->menu_main_page_label_volume, LV_CUSTOM_SYMBOL_VOLUME" 音量");
    lv_obj_add_style(ui->menu_main_page_label_volume, &menu_item_style, 0);

    ui->menu_main_page_label_brightness = lv_label_create(
        ui->menu_main_page_section_main);
    lv_label_set_text(ui->menu_main_page_label_brightness,LV_CUSTOM_SYMBOL_BRIGHTNESS" 背光");
    lv_obj_add_style(ui->menu_main_page_label_brightness, &menu_item_style, 0);

    ui->menu_main_page_label_llm = lv_label_create(ui->menu_main_page_section_main);
    lv_label_set_text(ui->menu_main_page_label_llm, LV_CUSTOM_SYMBOL_LLM" 大模型");
    lv_obj_add_style(ui->menu_main_page_label_llm, &menu_item_style, 0);

    ui->menu_main_page_seperator2 = lv_menu_separator_create(ui->menu_main_page);

    ui->menu_main_page_section_about = lv_menu_section_create(ui->menu_main_page);
    lv_obj_set_style_pad_hor(ui->menu_main_page_section_about, 5, 0);

    ui->menu_main_page_label_about = lv_label_create(ui->menu_main_page_section_about);
    lv_label_set_text(ui->menu_main_page_label_about, LV_CUSTOM_SYMBOL_ABOUT" 关于");
    lv_obj_add_style(ui->menu_main_page_label_about, &menu_item_style, 0);

    ui->menu_main_page_label_reset = lv_label_create(ui->menu_main_page_section_about);
    lv_label_set_text(ui->menu_main_page_label_reset, LV_CUSTOM_SYMBOL_RESET" 恢复出厂设置");
    lv_obj_set_style_bg_color(ui->menu_main_page_label_reset, lv_color_hex(0xAA0000), 0);
    lv_obj_add_style(ui->menu_main_page_label_reset, &menu_item_style, 0);

    lv_image_set_src(lv_obj_get_child(lv_menu_get_main_header_back_button(ui->menu), 0), LV_CUSTOM_SYMBOL_LEFT);

    // 创建网络设置页面

    lv_menu_set_page(ui->menu, ui->menu_main_page);
    lv_obj_add_event_cb(ui->menu, menu_event_callback, LV_EVENT_CLICKED, ui);
}
