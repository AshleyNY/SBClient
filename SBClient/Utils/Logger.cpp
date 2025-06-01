#include "Logger.h"

// 在 .cpp 文件中包含 DebugOverlay.h，因为这里需要使用 DebugOverlay::AddMessage

// 包含 <cstdio> 以使用 vsnprintf 进行格式化
#include <cstdio>
#include <vector> // 用于 vsnprintf 的缓冲区 (可选，但推荐)


bool Logger::s_Initialized = false;

void Logger::Init() {
    if (s_Initialized) return;
}

void Logger::Shutdown() {
    if (!s_Initialized) return;

    // 记录关闭消息到文件和 Overlay
    s_Initialized = false;
}


void Logger::ImLog(const std::string& message) {
    // 直接发送到 Overlay
    //ImguiHook::AddMessage(message);
}

void Logger::ImLogF(const char* format, ...) {
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, BUFFER_SIZE, format, args);
    va_end(args);

    // 直接发送到 Overlay
    //ImguiHook::AddMessage(std::string(buffer));
}