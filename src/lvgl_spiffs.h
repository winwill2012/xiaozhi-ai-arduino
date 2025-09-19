#ifndef XIAOZHI_AI_ARDUINO_LVGL_SPIFFS_H
#define XIAOZHI_AI_ARDUINO_LVGL_SPIFFS_H
#include "lvgl.h"

bool lv_init_spiffs();

bool ready_cb(lv_fs_drv_t *drv);

void *open_cb(lv_fs_drv_t *drv, const char *path, lv_fs_mode_t mode);

lv_fs_res_t close_cb(lv_fs_drv_t *drv, void *file_p);

lv_fs_res_t read_cb(lv_fs_drv_t *drv, void *file_p, void *buf, uint32_t btr, uint32_t *br);

lv_fs_res_t write_cb(lv_fs_drv_t *drv, void *file_p, const void *buf, uint32_t btw, uint32_t *bw);

lv_fs_res_t seek_cb(lv_fs_drv_t *drv, void *file_p, uint32_t pos, lv_fs_whence_t whence);

lv_fs_res_t tell_cb(lv_fs_drv_t *drv, void *file_p, uint32_t *pos_p);

void *dir_open_cb(lv_fs_drv_t *drv, const char *path);

lv_fs_res_t dir_read_cb(lv_fs_drv_t *drv, void *rddir_p, char *fn, uint32_t fn_len);

lv_fs_res_t dir_close_cb(lv_fs_drv_t *drv, void *rddir_p);

#endif //XIAOZHI_AI_ARDUINO_LVGL_SPIFFS_H
