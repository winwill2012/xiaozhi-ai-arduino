#ifndef COZE_LLMAGENT_H
#define COZE_LLMAGENT_H

#include "Arduino.h"
#include "CozeLLMAgent.h"
#define DELIMITER "|"

struct LLMTask
{
    char* message;
    size_t length;
};

// LLM状态列表
enum LLMState
{
    Init,
    CommandCompleted,
    ParamsCompleted,
    ResponseCompleted,
};

// LLM流式输出事件
enum LLMEvent
{
    Normal,
    Delimiter
};

class CozeLLMAgent
{
public:
    CozeLLMAgent();

    void publishTask(LLMTask task) const;

    void chat(const String& input);

    void processStreamOutput(String data);

    void processContent(String& content);

    void processPart(const String& input);

    void reset();

    // 状态机状态转移函数
    void stateTransfer(LLMState state, LLMEvent event);

private:
    LLMState _state = Init;
    // 回复内容
    String _response = "";
    // 命令类型
    String _command = "";
    // 附加参数
    String _params = "";
    String _ttsTextBuffer = "";
    bool _firstPacket;
    QueueHandle_t _taskQueue;
};


#endif
