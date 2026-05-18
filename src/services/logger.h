#pragma once

#include <QString>

namespace keyfreeze {

// 日志级别
enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error,
    Fatal
};

// 日志服务类
class Logger {
public:
    // 初始化日志系统
    static void init(LogLevel level = LogLevel::Info);
    
    // 日志输出
    static void debug(const QString& message, const QString& context = QString());
    static void info(const QString& message, const QString& context = QString());
    static void warning(const QString& message, const QString& context = QString());
    static void error(const QString& message, const QString& context = QString());
    static void fatal(const QString& message, const QString& context = QString());
    
    // 获取日志文件路径
    static QString getLogFilePath();
    
private:
    static void log(LogLevel level, const QString& message, const QString& context);
    static QString levelToString(LogLevel level);
    static QString getTimestamp();
};

} // namespace keyfreeze

// 便捷宏定义
#define KF_LOG_DEBUG(msg) keyfreeze::Logger::debug(msg, __FUNCTION__)
#define KF_LOG_INFO(msg) keyfreeze::Logger::info(msg, __FUNCTION__)
#define KF_LOG_WARNING(msg) keyfreeze::Logger::warning(msg, __FUNCTION__)
#define KF_LOG_ERROR(msg) keyfreeze::Logger::error(msg, __FUNCTION__)
#define KF_LOG_FATAL(msg) keyfreeze::Logger::fatal(msg, __FUNCTION__)
