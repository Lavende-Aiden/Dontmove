#pragma once

#include <QString>

namespace keyfreeze {

// 应用状态枚举
enum class AppMode {
    Idle,       // 空闲状态
    Locking,    // 正在锁定
    Locked,     // 已锁定
    Error       // 错误状态
};

// 输入设备类型
enum class InputDevice {
    Keyboard,
    Mouse,
    Both
};

// 热键动作
enum class HotkeyAction {
    ToggleLock,
    EmergencyUnlock,
    OpenSettings
};

// 应用配置结构
struct AppConfig {
    bool lockKeyboard = true;
    bool lockMouse = true;
    bool enableHotkey = false;
    int autoUnlockMinutes = 30;
    QString hotkeySequence = "Ctrl+Alt+F12";
    bool startMinimized = false;
};

// 会话信息
struct SessionInfo {
    bool isActive = false;
    int elapsedSeconds = 0;
    int remainingSeconds = 0;
};

} // namespace keyfreeze
