#include "Logger.h"

// �� .cpp �ļ��а��� DebugOverlay.h����Ϊ������Ҫʹ�� DebugOverlay::AddMessage

// ���� <cstdio> ��ʹ�� vsnprintf ���и�ʽ��
#include <cstdio>
#include <vector> // ���� vsnprintf �Ļ����� (��ѡ�����Ƽ�)


bool Logger::s_Initialized = false;

void Logger::Init() {
    if (s_Initialized) return;
}

void Logger::Shutdown() {
    if (!s_Initialized) return;

    // ��¼�ر���Ϣ���ļ��� Overlay
    s_Initialized = false;
}


void Logger::ImLog(const std::string& message) {
    // ֱ�ӷ��͵� Overlay
    //ImguiHook::AddMessage(message);
}

void Logger::ImLogF(const char* format, ...) {
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, BUFFER_SIZE, format, args);
    va_end(args);

    // ֱ�ӷ��͵� Overlay
    //ImguiHook::AddMessage(std::string(buffer));
}