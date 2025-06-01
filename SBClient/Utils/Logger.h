#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include <stdarg.h> 


class Logger {
public:
    // ��ʼ�� Logger (�����ļ��Ϳ��ܵ��������)
    static void Init();

    // �ر� Logger
    static void Shutdown();


    // ��־�� ImGui Overlay
    static void ImLog(const std::string& message);
    // ��ʽ����־�� ImGui Overlay
    static void ImLogF(const char* format, ...);

private:
    // �����ļ���־
    static std::ofstream s_LogFile;
    static std::mutex s_Mutex; // ����ͬ���ļ�д�����Ϣ���з���
    static bool s_Initialized;
};