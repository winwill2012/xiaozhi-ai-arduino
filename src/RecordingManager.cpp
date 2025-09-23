#include <asr/DoubaoASR.h>
#include "RecordingManager.h"
#include <Settings.h>
#include "Application.h"
#include "Utils.h"
#include "GlobalState.h"

RecordingManager::RecordingManager() {
    constexpr i2s_config_t i2s_config = {
            .mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_RX),
            .sample_rate = AUDIO_SAMPLE_RATE,
            .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
            .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT, // 这里的左右声道要和电路保持一致
            .communication_format = I2S_COMM_FORMAT_STAND_I2S,
            .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
            .dma_buf_count = 4,
            .dma_buf_len = 1024,
            .use_apll = false
    };
    constexpr i2s_pin_config_t pin_config = {
            .bck_io_num = MICROPHONE_I2S_BCLK,
            .ws_io_num = MICROPHONE_I2S_LRC,
            .data_out_num = -1,
            .data_in_num = MICROPHONE_I2S_DOUT
    };

    i2s_driver_install(MICROPHONE_I2S_NUM, &i2s_config, 0, nullptr);
    i2s_set_pin(MICROPHONE_I2S_NUM, &pin_config);
    i2s_zero_dma_buffer(MICROPHONE_I2S_NUM);
    _ringBuffer = xRingbufferCreate(1024 * 32, RINGBUF_TYPE_BYTEBUF);
}

[[noreturn]] void consumeBufferedData(void *arg) {
    size_t pxItemSize;
    bool firstPacket = true;
    auto lastPacketBuff = std::vector<uint8_t>();
    lastPacketBuff.push_back(0);
    while (true) {
        auto *buffer = static_cast<uint8_t *>(
                xRingbufferReceive(Application::recordingManager()->getRingBuffer(),
                                   &pxItemSize, pdMS_TO_TICKS(500))
        );
        if (buffer == nullptr) {
            if (!firstPacket) {
                Application::asr()->recognize(DoubaoASRTask(lastPacketBuff, firstPacket, true));
                firstPacket = true;
            }
        } else {
            Application::asr()->recognize(DoubaoASRTask(
                    std::vector<uint8_t>(buffer, buffer + pxItemSize),
                    firstPacket, false));
            if (firstPacket) {
                firstPacket = false;
            }
            vRingbufferReturnItem(Application::recordingManager()->getRingBuffer(), buffer);
        }
        vTaskDelay(1);
    }
}

[[noreturn]] void RecordingManager::begin() {
    size_t bytesRead;
    xTaskCreate(consumeBufferedData, "consumeBufferTask", 1024 * 16, this, 1, nullptr);
    std::vector<int16_t> buffer;
    while (true) {
        buffer.reserve(AUDIO_RECORDING_SAMPLE_NUMBER);
        xEventGroupWaitBits(GlobalState::getEventGroup(), GlobalState::getEventBits({Listening}),
                            false, false, portMAX_DELAY);
        const esp_err_t err = i2s_read(MICROPHONE_I2S_NUM, buffer.data(),
                                       AUDIO_RECORDING_SAMPLE_NUMBER * sizeof(int16_t),
                                       &bytesRead, portMAX_DELAY);
        if (err == ESP_OK) {
            // 开始录音就提前建立ASR连接，加快语音识别速度
            Application::asr()->connect();
            buffer.resize(bytesRead / sizeof(int16_t));
            // 16位PCM数据，转换成大端序字节流
            //                std::vector<uint8_t> uint8Buffer = int16ToUint8BigEndian(buffer);
            xRingbufferSend(_ringBuffer, buffer.data(), buffer.size() * sizeof(int16_t), portMAX_DELAY);
        }
        vTaskDelay(1);
    }
}
