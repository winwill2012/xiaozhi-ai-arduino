#ifndef DOUBAO_ASR_H
#define DOUBAO_ASR_H

#include <Arduino.h>
#include <WebSocketsClient.h>
#include <vector>

#define STT_TASK_COMPLETED_EVENT (1<<1)

// 默认头部
constexpr byte DoubaoTTSDefaultFullClientWsHeader[] = {0x11, 0x10, 0x10, 0x00};
constexpr byte DoubaoTTSDefaultAudioOnlyWsHeader[] = {0x11, 0x20, 0x10, 0x00};
constexpr byte DoubaoTTSDefaultLastAudioWsHeader[] = {0x11, 0x22, 0x10, 0x00};

struct DoubaoASRTask {
    DoubaoASRTask(const std::vector<uint8_t> &data, const bool firstPacket, const bool lastPacket)
            : data(data), firstPacket(firstPacket), lastPacket(lastPacket) {
    };

    // 音频数据
    std::vector<uint8_t> data;

    // 是否第一帧数据
    bool firstPacket;

    // 是否最后一帧数据
    bool lastPacket;
};

class DoubaoASR final : public WebSocketsClient {
public:
    DoubaoASR();

    void connect();

    void eventCallback(WStype_t type, uint8_t *payload, size_t length);

    void parseResponse(const uint8_t *response);

    void buildFullClientRequest();

    void buildAudioOnlyRequest(uint8_t *audio, size_t size, bool lastPacket);

    void recognize(DoubaoASRTask task);

private:
    // 音频数据
    std::vector<uint8_t> data;

    EventGroupHandle_t _eventGroup;
    bool _firstPacket;
    volatile bool _isConnecting = false;
    std::vector<uint8_t> _requestBuilder;
};

#endif
