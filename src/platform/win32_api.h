#pragma once

#include "common/types.h"

#include <Windows.h>
#include <QString>

namespace keyfreeze {
namespace platform {

// Windows API 封装类
class Win32Api {
public:
    // 输入控制
    static bool blockInput(bool enable);
    static bool isInputBlocked();
    
    // 热键管理
    static bool registerGlobalHotkey(int key, int modifiers, HWND hwnd, int id);
    static void unregisterGlobalHotkey(HWND hwnd, int id);
    
    // 权限检查
    static bool isRunningAsAdministrator();
    static bool requestAdministratorPrivileges();
    
    // 系统信息
    static QString getWindowsVersion();
    static bool isSystemLocked();
    
    // 紧急解锁钩子（低级键盘钩子）
    static HHOOK installLowLevelKeyboardHook(HOOKPROC callback);
    static void uninstallKeyboardHook(HHOOK hook);
    
    // 会话管理
    static void disableScreensaver(bool disable);
    static bool isScreensaverDisabled();
};

} // namespace platform
} // namespace keyfreeze
