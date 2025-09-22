#ifndef XIAOZHI_AI_ARDUION_GLOBALSTATE_H
#define XIAOZHI_AI_ARDUION_GLOBALSTATE_H

#include <Arduino.h>
#include <vector>

enum MachineState
{
    Sleep,
    NetworkConfigurationNotFound,
    NetworkConnecting,
    NetworkConnected,
    NetworkConnectFailed,
    Listening,
    Recognizing,
    Thinking,
    Speaking
};

class GlobalState
{
public:
    static void setConversationId(String conversationId);

    static EventGroupHandle_t getEventGroup();

    static String getConversationId();

    static EventBits_t getEventBits(const std::vector<MachineState>& states);

    static MachineState getState();

    static void setState(MachineState state);

private:
    static String conversationId;
    static MachineState machineState;
    static EventGroupHandle_t eventGroup;
    static String connectingWiFiMessage;
};


#endif
