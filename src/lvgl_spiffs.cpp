#include "lvgl_spiffs.h"
#include "lvgl.h"
#include "SPIFFS.h"

bool lv_init_spiffs() {
    if (!SPIFFS.begin(true)) {
        LV_LOG_ERROR("lvgl spiffs system init failed");
        return false;
    }
    static lv_fs_drv_t drv;
    lv_fs_drv_init(&drv);
    drv.letter = 'S';
    drv.cache_size = 0;
    drv.read_cb = read_cb;
    drv.write_cb = write_cb;
    drv.open_cb = open_cb;
    drv.seek_cb = seek_cb;
    drv.ready_cb = ready_cb;
    drv.close_cb = close_cb;
    drv.tell_cb = tell_cb;
    drv.dir_open_cb = dir_open_cb;
    drv.dir_close_cb = dir_close_cb;
    drv.dir_read_cb = dir_read_cb;
    drv.user_data = nullptr;

    lv_fs_drv_register(&drv);
    return true;
}

bool ready_cb(lv_fs_drv_t *drv) {
    return SPIFFS.begin(false);
}

void *open_cb(lv_fs_drv_t *drv, const char *path, lv_fs_mode_t mode) {
    auto spiffs_mode = FILE_READ;
    if (mode == LV_FS_MODE_WR) {
        spiffs_mode = FILE_WRITE;
    }
    File *file = new File();
    *file = SPIFFS.open(path, spiffs_mode);
    return file;
}

lv_fs_res_t close_cb(lv_fs_drv_t *drv, void *file_p) {
    const auto file = static_cast<File *>(file_p);
    file->close();
    return LV_FS_RES_OK;
}

lv_fs_res_t read_cb(lv_fs_drv_t *drv, void *file_p, void *buf, uint32_t btr, uint32_t *br) {
    const auto file = static_cast<File *>(file_p);
    *br = file->readBytes(static_cast<char *>(buf), btr);
    return static_cast<int32_t>(*br) < 0 ? LV_FS_RES_UNKNOWN : LV_FS_RES_OK;
}

lv_fs_res_t write_cb(lv_fs_drv_t *drv, void *file_p, const void *buf, uint32_t btw, uint32_t *bw) {
    const auto file = static_cast<File *>(file_p);
    *bw = file->write(static_cast<const uint8_t *>(buf), btw);
    return static_cast<int32_t>(*bw) < 0 ? LV_FS_RES_UNKNOWN : LV_FS_RES_OK;
}

lv_fs_res_t seek_cb(lv_fs_drv_t *drv, void *file_p, uint32_t pos, lv_fs_whence_t whence) {
    const auto file = static_cast<File *>(file_p);
    SeekMode mode;
    if (whence == LV_FS_SEEK_SET) {
        mode = SeekSet;
    } else if (whence == LV_FS_SEEK_CUR) {
        mode = SeekCur;
    } else {
        mode = SeekEnd;
    }
    return file->seek(pos, mode) ? LV_FS_RES_OK : LV_FS_RES_UNKNOWN;
}

lv_fs_res_t tell_cb(lv_fs_drv_t *drv, void *file_p, uint32_t *pos_p) {
    const auto file = static_cast<File *>(file_p);
    *pos_p = file->position();
    return LV_FS_RES_OK;
}

void *dir_open_cb(lv_fs_drv_t *drv, const char *path) {
    return nullptr;
}

lv_fs_res_t dir_read_cb(lv_fs_drv_t *drv, void *rddir_p, char *fn, uint32_t fn_len) {
    return LV_FS_RES_NOT_IMP;
}

lv_fs_res_t dir_close_cb(lv_fs_drv_t *drv, void *rddir_p) {
    return LV_FS_RES_NOT_IMP;
}
