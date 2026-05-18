#pragma once

#include <QString>

namespace keyfreeze {
namespace constants {

// 应用信息
inline const QString kAppName = "Dontmove";
inline const QString kAppVersion = "0.1.0";
inline const QString kOrganization = "Aiden Team";

// 默认配置
inline const int kDefaultAutoUnlockMinutes = 30;
inline const QString kDefaultHotkey = "Ctrl+Alt+F12";

// 紧急解锁
inline const int kEmergencyUnlockKeyCount = 3;  // 连续按 Esc 次数
inline const int kEmergencyUnlockTimeWindowMs = 2000;  // 时间窗口（毫秒）

// 会话限制
inline const int kMaxSessionMinutes = 240;  // 最大会话时长（4小时）
inline const int kMinSessionMinutes = 1;    // 最小会话时长

// 日志
inline const QString kLogFileName = "dontmove.log";
inline const int kMaxLogFileSizeMB = 10;
inline const int kMaxLogFileCount = 5;

// 设置（与 kAppName / kOrganization 保持一致，无需单独定义）
// kSettingsOrganization 和 kSettingsApplication 已移除，直接在 settings_manager.cpp 中使用 kAppName/kOrganization

} // namespace constants
} // namespace keyfreeze
