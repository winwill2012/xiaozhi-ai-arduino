#ifndef LVGL_GUI_H
#define LVGL_GUI_H

#include "src/misc/lv_types.h"
#include "lvgl.h"

// 声明阿里巴巴中文字体（通知支持下方所有图标）
LV_FONT_DECLARE(AlibabaPuHuiTi_Regular_15)

#define LV_CUSTOM_SYMBOL_DOWN          "\xEE\x99\x9E" // 向下的箭头
#define LV_CUSTOM_SYMBOL_LEFT          "\xEE\x98\xA3" // 向左的箭头
#define LV_CUSTOM_SYMBOL_WIFI          "\xEE\xA3\x81" // WiFi网络
#define LV_CUSTOM_SYMBOL_NO_WIFI       "\xEE\x9E\xB3" // 无WiFi网络
#define LV_CUSTOM_SYMBOL_VOLUME        "\xEE\x98\xA6" // 音量
#define LV_CUSTOM_SYMBOL_SPEED         "\xEE\x95\xB5" // 说话速度
#define LV_CUSTOM_SYMBOL_LLM           "\xEE\x98\x81" // 大模型
#define LV_CUSTOM_SYMBOL_BRIGHTNESS    "\xEE\x98\x89" // 背光亮度
#define LV_CUSTOM_SYMBOL_VOICE         "\xEE\x98\xB3" // 声音
#define LV_CUSTOM_SYMBOL_PERSONA       "\xEF\x80\x81" // 性格
#define LV_CUSTOM_SYMBOL_ABOUT         "\xEE\x99\x85" // 关于
#define LV_CUSTOM_SYMBOL_SETTINGS      "\xEE\x9D\xA5" // 设置
#define LV_CUSTOM_SYMBOL_RESET         "\xEE\x99\x88" // 重置
#define LV_CUSTOM_SYMBOL_LOADING       "\xEE\x98\xBA" // 加载
#define LV_CUSTOM_SYMBOL_MORE          "\xEE\x98\x8D" // 更多
#define LV_CUSTOM_SYMBOL_MIC           "\xEE\x98\x9E" // 麦克风

#define STATUS_BAR_HEIGHT 25     // 顶部状态栏的高度
#define SPEAK_BUTTON_HEIGHT 35   // 底部说话按钮的高度

// 定义一个结构体，用于管理整个项目UI上用到的所有对象
typedef struct {
    // 顶部状态栏相关组件
    lv_obj_t *status_bar;
    lv_obj_t *status_bar_time_label;
    lv_obj_t *status_bar_state_label;
    lv_obj_t *status_bar_wifi_image;

    // 首页相关组件
    lv_obj_t *screen_home;
    lv_obj_t *screen_home_message_list;
    lv_obj_t *screen_home_speak_button;
    lv_obj_t *screen_home_speak_button_label;

    // 首页顶部下拉设置框相关组件
    lv_obj_t *control_bar;
    bool control_bar_active;
    lv_obj_t *control_bar_cont;
    lv_obj_t *control_bar_volume_image;
    lv_obj_t *control_bar_volume_slider;
    lv_obj_t *control_bar_brightness_image;
    lv_obj_t *control_bar_brightness_slider;
    lv_obj_t *control_bar_more_settings_button;
    lv_obj_t *control_bar_more_settings_button_label;

    // 设置页相关组件
    lv_obj_t *screen_settings;
    lv_obj_t *menu;
    lv_obj_t *menu_main_page;
    lv_obj_t *menu_main_page_seperator1;
    lv_obj_t *menu_main_page_section_main;
    lv_obj_t *menu_main_page_label_network;
    lv_obj_t *menu_main_page_label_llm;
    lv_obj_t *menu_main_page_label_about;
    lv_obj_t *menu_main_page_label_reset;

    // 设置页面->网络设置二级菜单相关组件
    lv_obj_t *menu_network_page;
    lv_obj_t *menu_network_page_cont1;
    lv_obj_t *menu_network_page_tips_label;
    lv_obj_t *menu_network_page_refresh_button;
    lv_obj_t *menu_network_page_refresh_button_label;
    lv_obj_t *menu_network_page_cont2;
    lv_obj_t *menu_network_page_wifi_list;
    const char *clicked_wifi_name;
    lv_obj_t *menu_network_page_wifi_list_loading_image;
    lv_obj_t *menu_network_page_keyboard_mask; // 键盘遮罩层
    lv_obj_t *menu_network_page_password_dialog;
    lv_obj_t *menu_network_page_password_tips_label; // WiFi密码输入框
    lv_obj_t *menu_network_page_password_textarea; // WiFi密码输入框
    lv_obj_t *button_cont;
    lv_obj_t *confirm_button;
    lv_obj_t *confirm_button_label;
    lv_obj_t *cancel_button;
    lv_obj_t *cancel_button_label;
    lv_obj_t *menu_network_page_keyboard; // 键盘遮罩层
    lv_obj_t *menu_network_page_password_confirm_button;
    lv_obj_t *menu_network_page_password_cancel_button;

    // 设置页->大模型设置二级菜单相关组件
    lv_obj_t *menu_llm_page;
    lv_obj_t *menu_llm_page_cont1;
    lv_obj_t *menu_llm_page_voice_label;
    lv_obj_t *menu_llm_page_voice_dropdown;
    lv_obj_t *menu_llm_page_cont2;
    lv_obj_t *menu_llm_page_speed_label;
    lv_obj_t *menu_llm_page_speed_slider;
    lv_obj_t *menu_llm_page_cont3;
    lv_obj_t *menu_llm_page_persona_label;
    lv_obj_t *menu_llm_page_persona_dropdown;

    lv_obj_t *menu_main_page_seperator2;
    lv_obj_t *menu_main_page_section_about;

    // 设置页->恢复出厂设置二级页面相关组件
    lv_obj_t *menu_page_reset;
    lv_obj_t *menu_page_reset_button;
    lv_obj_t *menu_page_reset_button_label;
    lv_obj_t *menu_page_reset_msgbox;
    lv_obj_t *menu_page_reset_msgbox_confirm_button;
    lv_obj_t *menu_page_reset_msgbox_cancel_button;

    // 设置页->关于二级菜单相关组件
    lv_obj_t *menu_page_about;
    lv_obj_t *menu_page_about_label1;
    lv_obj_t *menu_page_about_label2;
} lv_screens_info;

extern lv_screens_info lv_ui;

void setup_ui();

#endif //LVGL_GUI_H
