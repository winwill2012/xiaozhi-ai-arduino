#include "CozeLLMAgent.h"
#include <HTTPClient.h>
#include <utility>
#include "ArduinoJson.h"
#include <Arduino.h>
#include "Utils.h"
#include "Settings.h"
#include "GlobalState.h"
#include "LvglDisplay.h"
#include "Application.h"
#include "IOT.h"

static auto TAG = "CozeAgent";

CozeLLMAgent::CozeLLMAgent() {
    _firstPacket = true;
    _taskQueue = xQueueCreate(5, sizeof(LLMTask));
    xTaskCreate([](void *ptr) {
        auto *instance = static_cast<CozeLLMAgent *>(ptr);
        LLMTask task{};
        while (true) {
            if (xQueueReceive(instance->_taskQueue, &task, pdMS_TO_TICKS(1)) == pdTRUE) {
                instance->chat(String(task.message, task.length));
                free(task.message);
            }
            vTaskDelay(1);
        }
    }, "llmTaskLoop", 1024 * 8, this, 1, nullptr);
}

void CozeLLMAgent::chat(const String &input) {
    if (input == "") return;
    ESP_LOGI(TAG, "发送消息到Coze智能体：%s", input.c_str());
    GlobalState::setState(Thinking);
    reset();
    HTTPClient http;
    http.begin("https://api.coze.cn/v3/chat?conversation_id=" + GlobalState::getConversationId());
    http.addHeader("Authorization", ("Bearer " + Settings::getCozeToken()).c_str());
    http.addHeader("Content-Type", "application/json");
    // 构建请求体
    JsonDocument requestBody;
    requestBody.clear();
    requestBody["stream"] = true;
    std::map<std::string, std::string> personaMap = Settings::getPersonaMap();
    requestBody["bot_id"] = personaMap[Settings::getCurrentPersona().c_str()];
    requestBody["user_id"] = getChipId(nullptr);
    const JsonArray additionalMessages = requestBody["additional_messages"].to<JsonArray>();
    const JsonObject message = additionalMessages.add<JsonObject>();
    message["content_type"] = "text";
    message["content"] = input;
    message["role"] = "user";
    std::string requestBodyStr;
    serializeJson(requestBody, requestBodyStr);
    const int httpResponseCode = http.POST(requestBodyStr.c_str());
    if (httpResponseCode > 0) {
        // 开始调用智能体接口就创建TTS连接，提高语音合成相应速度
        Application::tts()->connect();
        String lastEvent;
        WiFiClient *stream = http.getStreamPtr();
        String line = "";
        while (stream->connected() || stream->available()) {
            // 等待数据流有新的数据可读
            while (!stream->available()) {
                vTaskDelay(pdMS_TO_TICKS(10));
            }
            line = stream->readStringUntil('\n');
            if (!line.isEmpty()) {
                // 表示大模型回复已经完全结束
                if (line.compareTo("event:conversation.message.completed") == 0
                    && lastEvent.compareTo("event:conversation.message.delta") == 0) {
                    ESP_LOGI(TAG, "====================大模型调用结束==================");
                    ESP_LOGI(TAG, "命令: %s", _command.c_str());
                    ESP_LOGI(TAG, "参数: %s", _params.c_str());
                    ESP_LOGI(TAG, "回复: %s", _response.c_str());
                    if (_ttsTextBuffer != "") {
                        ESP_LOGI(TAG, "最后一条消息，继续语音合成: %s", _ttsTextBuffer.c_str());
                        Application::tts()->synth(_ttsTextBuffer, true);
                    } else {
                        Application::tts()->disconnect();
                    }
                    break;
                }
                // 返回true表示强制终止
                processStreamOutput(line);
                if (line.startsWith("event:")) {
                    lastEvent = line;
                }
            }
            vTaskDelay(pdMS_TO_TICKS(1));
        }
    } else {
        ESP_LOGE(TAG, "请求智能体接口失败: %s", httpResponseCode);
    }
    http.end();
}

void CozeLLMAgent::processStreamOutput(String data) {
    // 只处理data开头，并且是助手回答的数据类型
    if (!data.startsWith("data:") || data.indexOf(R"("role":"assistant","type":"answer")") < 0) {
        return;
    }
    data.replace("data:", "");
    JsonDocument document;
    document.clear();
    const DeserializationError error = deserializeJson(document, data);
    if (error != DeserializationError::Ok) {
        ESP_LOGE(TAG, "解析智能体请求结果失败: %s", data.c_str());
        return;
    }
    String content = document["content"];
    const String conversationId = document["conversation_id"];
    GlobalState::setConversationId(conversationId);
    return processContent(content);
}

void CozeLLMAgent::reset() {
    _state = Init;
    _response = "";
    _ttsTextBuffer = "";
    _command = "";
    _params = "";
    _firstPacket = true;
}

// 处理增量消息
void CozeLLMAgent::processContent(String &content) {
    if (content.isEmpty()) {
        return;
    }
    const int index = content.indexOf(DELIMITER);
    // 如果不包含分隔符，状态不会发生变化
    if (index < 0) {
        processPart(content);
        content = "";
    } else // 如果包含分隔符
    {
        const String leftPart = content.substring(0, index);
        if (!leftPart.isEmpty()) {
            processPart(leftPart);
        }
        // 再执行状态转移
        stateTransfer(_state, Delimiter);
        content = content.substring(index + 1);
        processContent(content); // 然后递归处理右边部分
    }
    // 回复的内容里面包含一些可以断句的标点符号时，直接发送给TTS进行语音合成，降低响应延迟
    while (true) {
        const std::pair<int, size_t> delimiterIndex = findMinIndexOfDelimiter(_ttsTextBuffer);
        if (delimiterIndex.first >= 0) {
            Application::tts()->synth(_ttsTextBuffer.substring(0, delimiterIndex.first),
                                      false);
            _ttsTextBuffer = _ttsTextBuffer.substring(delimiterIndex.first + delimiterIndex.second);
        } else {
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

void CozeLLMAgent::processPart(const String &input) {
    switch (_state) {
        case Init:
            _command += input;
            break;
        case CommandCompleted:
            _params += input;
            break;
        case ParamsCompleted:
            _response += input;
            _ttsTextBuffer += input;
            LvglDisplay::updateChatText(_firstPacket, _response.c_str());
            if (_firstPacket) {
                _firstPacket = false;
            }
            break;
        default:
            break;
    }
}

void CozeLLMAgent::publishTask(const LLMTask task) const {
    if (xQueueSend(_taskQueue, &task, portMAX_DELAY) == pdFALSE) {
        ESP_LOGE(TAG, "发送智能体结果到语音合成队列失败: %s", task.message);
        free(task.message);
    }
}

void CozeLLMAgent::stateTransfer(const LLMState state, const LLMEvent event) {
    switch (state) {
        case Init:
            switch (event) {
                case Normal:
                    _state = Init;
                    break;
                case Delimiter:
                    _state = CommandCompleted;
                    break;
            }
            break;
        case CommandCompleted:
            switch (event) {
                case Normal:
                    _state = CommandCompleted;
                    break;
                case Delimiter:
                    _state = ParamsCompleted;
                    IOT::handle(_command, _params);
                    break;
            }
            break;
        case ParamsCompleted:
            switch (event) {
                case Normal:
                    _state = ParamsCompleted;
                    break;
                case Delimiter:
                    _state = ResponseCompleted;
                    break;
            }
            break;
        default:
            break;
    }
}
