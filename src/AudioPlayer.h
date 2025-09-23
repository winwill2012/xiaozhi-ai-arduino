#ifndef ESP32_AI_ASSISTANT_AUDIOPLAYER_H
#define ESP32_AI_ASSISTANT_AUDIOPLAYER_H

#include "Arduino.h"
#include <vector>
#include <driver/i2s.h>

#define MAX98357_I2S_NUM  I2S_NUM_0
#define SAMPLE_RATE       16000
#define MAX98357_DOUT     38
#define MAX98357_LRC      40
#define MAX98357_BCLK     39

struct PlayAudioTask {
    size_t length;
    int16_t *data;
};

class AudioPlayer {
public:
    AudioPlayer();

    void begin();

    void stop() const;

    void reset();

    static std::vector<int32_t> adjustVolume(const std::vector<int16_t> &input);

    static std::vector<int16_t> adjustVolume2(const std::vector<int16_t> &input);

    void publishTask(PlayAudioTask task) const;

    QueueHandle_t getTaskQueue() const { return _taskQueue; };

private:
    QueueHandle_t _taskQueue;
    i2s_config_t _max98357_i2s_config{};
    i2s_pin_config_t _max98357_i2s_pin_config{};
};


#endif //ESP32_AI_ASSISTANT_AUDIOPLAYER_H
