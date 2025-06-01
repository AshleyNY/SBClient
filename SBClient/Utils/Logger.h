#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include <stdarg.h> 


class Logger {
public:
    // 初始化 Logger (包括文件和可能的其他输出)
    static void Init();

    // 关闭 Logger
    static void Shutdown();


    // 日志到 ImGui Overlay
    static void ImLog(const std::string& message);
    // 格式化日志到 ImGui Overlay
    static void ImLogF(const char* format, ...);

private:
    // 用于文件日志
    static std::ofstream s_LogFile;
    static std::mutex s_Mutex; // 用于同步文件写入和消息队列访问
    static bool s_Initialized;
};