#include "AudioPlayer.h"
#include <Settings.h>
#include "GlobalState.h"
#include "Application.h"
#include "driver/i2s.h"

static auto TAG = "AudioPlayer";

AudioPlayer::AudioPlayer() {
    _taskQueue = xQueueCreate(30, sizeof(PlayAudioTask));
}

void playAudio(void *ptr) {
    PlayAudioTask task{};
    size_t bytes_written;
    while (true) {
        if (xQueueReceive(Application::audioPlayer()->getTaskQueue(),
                          &task, portMAX_DELAY) == pdTRUE) {
            GlobalState::setState(Speaking);
            // 调节音量后，16位PCM直接转换成32位PCM音频
            std::vector<int32_t> output = AudioPlayer::adjustVolume(
                std::vector<int16_t>(task.data, task.data + task.length));
            const esp_err_t result = i2s_write(MAX98357_I2S_NUM,
                                               output.data(),
                                               output.size() * sizeof(int32_t),
                                               &bytes_written,
                                               portMAX_DELAY);
            free(task.data);
            if (result != ESP_OK) {
                ESP_LOGE(TAG, "通过I2S播放音频数据失败, errorCode: %d", result);
            }

            // 处于播放状态，并且语音已经播放完成，则进入待机模式
            if (uxQueueMessagesWaiting(Application::audioPlayer()->getTaskQueue()) == 0
                && GlobalState::getState() == Speaking) {
                ESP_LOGI(TAG, "音频播放完毕，进入待命状态");
                GlobalState::setState(Sleep);
            }
        }
        vTaskDelay(1);
    }
}

void AudioPlayer::begin() {
    _max98357_i2s_config = {
        .mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // 中断优先级，如果对实时性要求高，可以调高优先级
        .dma_buf_count = 4,
        .dma_buf_len = 1024,
        .tx_desc_auto_clear = true
    };

    _max98357_i2s_pin_config = {
        .bck_io_num = MAX98357_BCLK,
        .ws_io_num = MAX98357_LRC,
        .data_out_num = MAX98357_DOUT,
        .data_in_num = -1
    };

    i2s_driver_install(MAX98357_I2S_NUM, &_max98357_i2s_config, 0, nullptr);
    i2s_set_pin(MAX98357_I2S_NUM, &_max98357_i2s_pin_config);
    xTaskCreate(playAudio, "playAudio", 4096, nullptr, 1, nullptr);
}

void AudioPlayer::stop() const {
    i2s_stop(MAX98357_I2S_NUM);
    i2s_zero_dma_buffer(MAX98357_I2S_NUM);
    ESP_LOGI(TAG, "I2S卸载完毕==================");
    while (uxQueueMessagesWaiting(_taskQueue) > 0) {
        PlayAudioTask task{};
        if (xQueueReceive(_taskQueue, &task, 0) == pdTRUE) {
            free(task.data);
        }
    }
}

void AudioPlayer::reset() {
    i2s_zero_dma_buffer(MAX98357_I2S_NUM);
    const error_t err = i2s_start(MAX98357_I2S_NUM);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2S启动失败: %d", err);
    }
    ESP_LOGI(TAG, "I2S准备完毕===================");
}

std::vector<int32_t> AudioPlayer::adjustVolume(const std::vector<int16_t> &input) {
    const double volumeRatio = Settings::getCurrentSpeakVolumeRatio(); // 当前设置的音量大小，范围[0, 1.0]
    // 因为人耳对音量的改变的感知不是线性的，低音量时的音量增加感觉更明显，音量大了感知会变缓，所以进行平方运算
    const int32_t factor = pow(volumeRatio, 2) * 65536;
    std::vector<int32_t> output(input.size());
    for (int i = 0; i < input.size(); i++) {
        int64_t data = static_cast<int64_t>(input[i]) * factor;
        if (data > INT32_MAX) {
            data = INT32_MAX;
        } else if (data < INT32_MIN) {
            data = INT32_MIN;
        }
        output[i] = static_cast<int32_t>(data);
    }
    return output;
}

void AudioPlayer::publishTask(const PlayAudioTask task) const {
    if (xQueueSend(_taskQueue, &task, portMAX_DELAY) != pdPASS) {
        ESP_LOGE(TAG, "发送音频播放任务到队列失败，音频长度： %d bytes", task.length);
        free(task.data); // 发送到队列失败，则生产者负责将内存回收
    }
}
