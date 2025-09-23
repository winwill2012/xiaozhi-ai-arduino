#ifndef IOT_AI_CONTROLLER_RECORDING_H
#define IOT_AI_CONTROLLER_RECORDING_H

#include <freertos/ringbuf.h>

#define MICROPHONE_I2S_NUM             I2S_NUM_1
#define AUDIO_SAMPLE_RATE              16000
#define AUDIO_RECORDING_SAMPLE_NUMBER  160    // 10ms音频样本数
#define MICROPHONE_I2S_BCLK            42
#define MICROPHONE_I2S_LRC             2
#define MICROPHONE_I2S_DOUT            1

class RecordingManager {
public:
    RecordingManager();

    [[noreturn]] void begin();

    RingbufHandle_t getRingBuffer() const { return _ringBuffer; };

private:
    RingbufHandle_t _ringBuffer;
};

#endif //IOT_AI_CONTROLLER_RECORDING_H
