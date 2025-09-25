#include "screen_settings_llm.h"

#include "gui.h"
#include "lvgl.h"

void setup_settings_llm() {
    lv_ui.menu_llm_page = lv_menu_page_create(lv_ui.menu, "智能体设置");

    // 音色
    lv_ui.menu_llm_page_cont1 = lv_menu_cont_create(lv_ui.menu_llm_page);
    lv_obj_set_style_pad_all(lv_ui.menu_llm_page_cont1, 10, 0);
    lv_ui.menu_llm_page_voice_label = lv_label_create(lv_ui.menu_llm_page_cont1);
    lv_label_set_text(lv_ui.menu_llm_page_voice_label, "音色");
    lv_ui.menu_llm_page_voice_dropdown = lv_dropdown_create(lv_ui.menu_llm_page_cont1);
    lv_obj_set_style_pad_hor(lv_ui.menu_llm_page_voice_dropdown, 10, 0);
    lv_obj_set_flex_grow(lv_ui.menu_llm_page_voice_dropdown, 1);

    lv_dropdown_set_options(lv_ui.menu_llm_page_voice_dropdown, "湾湾小何\n御姐\n武则天");
    lv_dropdown_set_symbol(lv_ui.menu_llm_page_voice_dropdown, LV_CUSTOM_SYMBOL_DOWN);

    // 语速
    lv_ui.menu_llm_page_cont2 = lv_menu_cont_create(lv_ui.menu_llm_page);
    lv_obj_set_style_pad_all(lv_ui.menu_llm_page_cont2, 10, 0);
    lv_ui.menu_llm_page_speed_label = lv_label_create(lv_ui.menu_llm_page_cont2);
    lv_label_set_text(lv_ui.menu_llm_page_speed_label, "语速");
    lv_ui.menu_llm_page_speed_slider = lv_slider_create(lv_ui.menu_llm_page_cont2);
    lv_slider_set_range(lv_ui.menu_llm_page_speed_slider, 8, 20);
    lv_obj_set_height(lv_ui.menu_llm_page_speed_slider, 5);
    lv_obj_set_style_pad_hor(lv_ui.menu_llm_page_speed_slider, 10, 0);
    lv_obj_set_flex_grow(lv_ui.menu_llm_page_speed_slider, 1);

    // 性格
    lv_ui.menu_llm_page_cont3 = lv_menu_cont_create(lv_ui.menu_llm_page);
    lv_obj_set_style_pad_all(lv_ui.menu_llm_page_cont3, 10, 0);
    lv_ui.menu_llm_page_persona_label = lv_label_create(lv_ui.menu_llm_page_cont3);
    lv_label_set_text(lv_ui.menu_llm_page_persona_label, "性格");
    lv_ui.menu_llm_page_persona_dropdown = lv_dropdown_create(lv_ui.menu_llm_page_cont3);
    lv_obj_set_style_pad_hor(lv_ui.menu_llm_page_persona_dropdown, 10, 0);
    lv_obj_set_flex_grow(lv_ui.menu_llm_page_persona_dropdown, 1);

    lv_dropdown_set_options(lv_ui.menu_llm_page_persona_dropdown, "温柔\n泼辣");
    lv_dropdown_set_symbol(lv_ui.menu_llm_page_persona_dropdown, LV_CUSTOM_SYMBOL_DOWN);
}
