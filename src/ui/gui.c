#include "lvgl.h"
#include "gui.h"
#include "screen_home.h"
#include "screen_settings.h"

void setup_ui() {
    // 创建状态栏
    lv_ui.status_bar = lv_obj_create(lv_layer_top());
    lv_obj_set_size(lv_ui.status_bar, LV_HOR_RES, STATUS_BAR_HEIGHT);
    // 状态栏不可滚动
    lv_obj_remove_flag(lv_ui.status_bar, LV_OBJ_FLAG_SCROLLABLE);
    // 移除边框
    lv_obj_set_style_border_width(lv_ui.status_bar, 0, 0);
    // 设置颜色渐变起始颜色值
    lv_obj_set_style_bg_color(lv_ui.status_bar, lv_color_hex(0xff3300), 0);
    // 设置颜色渐变终止颜色值
    lv_obj_set_style_bg_grad_color(lv_ui.status_bar, lv_color_hex(0x0073e6), 0);
    // 渐变防方向水平
    lv_obj_set_style_bg_grad_dir(lv_ui.status_bar, LV_GRAD_DIR_HOR, 0);
    // 设置字体
    lv_obj_set_style_text_font(lv_ui.status_bar, &AlibabaPuHuiTi_Regular_15, 0);
    // 去除圆角
    lv_obj_set_style_radius(lv_ui.status_bar, 0, 0);
    // 设置布局方式为弹性布局，因为它的三个子节点（时间标签、状态标签、wifi标识都水平弹性布局的）
    lv_obj_set_layout(lv_ui.status_bar, LV_LAYOUT_FLEX);
    // 弹性布局方向为行布局（水平排列）
    lv_obj_set_flex_flow(lv_ui.status_bar, LV_FLEX_FLOW_ROW);
    // 设置对齐方式
    lv_obj_set_flex_align(lv_ui.status_bar, LV_FLEX_ALIGN_SPACE_BETWEEN,
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // 创建时间标签
    lv_ui.status_bar_time_label = lv_label_create(lv_ui.status_bar);
    lv_label_set_text(lv_ui.status_bar_time_label, "00:00");

    // 创建状态标签
    lv_ui.status_bar_state_label = lv_label_create(lv_ui.status_bar);
    lv_label_set_text(lv_ui.status_bar_state_label, "正在聆听...");

    // 创建wifi标识图片
    lv_ui.status_bar_wifi_image = lv_image_create(lv_ui.status_bar);
    lv_obj_set_size(lv_ui.status_bar_wifi_image, 30, 30);
    lv_image_set_src(lv_ui.status_bar_wifi_image, LV_CUSTOM_SYMBOL_NO_WIFI);

    // 创建完顶部状态栏后，加载首页主题部分组件
    setup_screen_home();
    // 默认显示首页界面
    setup_screen_settings();
}
