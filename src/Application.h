#ifndef ESP32_AI_ASSISTANT_APPLICATION_H
#define ESP32_AI_ASSISTANT_APPLICATION_H

#include "tts/DoubaoTTS.h"
#include "llm/CozeLLMAgent.h"
#include "RecordingManager.h"
#include "AudioPlayer.h"
#include "asr/DoubaoASR.h"

class Application {
public:
    static Application &getInstance() {
        static auto *instance = new Application();
        return *instance;
    }

    Application(const Application &) = delete;

    Application &operator=(const Application &) = delete;

    void begin() const;

    static void showMemoryInfo();

    static DoubaoTTS *tts() { return getInstance()._ttsClient; }

    static DoubaoASR *asr() { return getInstance()._asrClient; }

    static CozeLLMAgent *llm() { return getInstance()._llmAgent; }

    static RecordingManager *recordingManager() { return getInstance()._recordingManager; }

    static AudioPlayer *audioPlayer() { return getInstance()._audioPlayer; }

private:
    Application();

    DoubaoTTS *_ttsClient = nullptr;
    DoubaoASR *_asrClient = nullptr;
    CozeLLMAgent *_llmAgent = nullptr;
    AudioPlayer *_audioPlayer = nullptr;
    RecordingManager *_recordingManager = nullptr;
};


#endif //ESP32_AI_ASSISTANT_APPLICATION_H
