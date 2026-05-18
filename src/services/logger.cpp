#include "logger.h"

#include "common/constants.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QtDebug>

namespace keyfreeze {

// 静态成员初始化
static LogLevel g_logLevel = LogLevel::Info;
static QString g_logFilePath;

void Logger::init(LogLevel level) {
    g_logLevel = level;
    
    // 创建日志目录
    QString logDir = QCoreApplication::applicationDirPath() + "/logs";
    QDir().mkpath(logDir);
    
    g_logFilePath = logDir + "/" + constants::kLogFileName;
    
    info("Logger initialized", "Logger");
}

void Logger::debug(const QString& message, const QString& context) {
    if (g_logLevel <= LogLevel::Debug) {
        log(LogLevel::Debug, message, context);
    }
}

void Logger::info(const QString& message, const QString& context) {
    if (g_logLevel <= LogLevel::Info) {
        log(LogLevel::Info, message, context);
    }
}

void Logger::warning(const QString& message, const QString& context) {
    if (g_logLevel <= LogLevel::Warning) {
        log(LogLevel::Warning, message, context);
    }
}

void Logger::error(const QString& message, const QString& context) {
    log(LogLevel::Error, message, context);
}

void Logger::fatal(const QString& message, const QString& context) {
    log(LogLevel::Fatal, message, context);
}

QString Logger::getLogFilePath() {
    return g_logFilePath;
}

void Logger::log(LogLevel level, const QString& message, const QString& context) {
    QString logLine = QString("[%1] [%2] [%3] %4")
                          .arg(getTimestamp())
                          .arg(levelToString(level))
                          .arg(context.isEmpty() ? "General" : context)
                          .arg(message);
    
    // 输出到控制台
    switch (level) {
        case LogLevel::Debug:
            qDebug().noquote() << logLine;
            break;
        case LogLevel::Info:
            qInfo().noquote() << logLine;
            break;
        case LogLevel::Warning:
            qWarning().noquote() << logLine;
            break;
        case LogLevel::Error:
        case LogLevel::Fatal:
            qCritical().noquote() << logLine;
            break;
    }
    
    // 输出到文件
    QFile file(g_logFilePath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << logLine << Qt::endl;
        file.close();
    }
}

QString Logger::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::Debug:   return "DEBUG";
        case LogLevel::Info:    return "INFO";
        case LogLevel::Warning: return "WARNING";
        case LogLevel::Error:   return "ERROR";
        case LogLevel::Fatal:   return "FATAL";
        default:                return "UNKNOWN";
    }
}

QString Logger::getTimestamp() {
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
}

} // namespace keyfreeze
