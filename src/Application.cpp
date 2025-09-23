#include "Application.h"
#include <asr/DoubaoASR.h>

#include "IOT.h"

Application::Application() {
    IOT::begin();
    IOT::turnOffRgb();
    _ttsClient = new DoubaoTTS();
    _llmAgent = new CozeLLMAgent();
    _asrClient = new DoubaoASR();
    _audioPlayer = new AudioPlayer();
    _recordingManager = new RecordingManager();
}

void Application::begin() const {
    _audioPlayer->begin();
    _recordingManager->begin();
}

void Application::showMemoryInfo() {
    xTaskCreate([](void *ptr) {
        while (true) {
            ESP_LOGD("Application", "Free ram: %d", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
            ESP_LOGD("Application", "Free psram: %d", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }, "showMemoryInfo", 2048, nullptr, 1, nullptr);
}
