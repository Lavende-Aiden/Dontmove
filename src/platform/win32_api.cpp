#include "win32_api.h"

#include <QCoreApplication>
#include <shellapi.h>
#include <VersionHelpers.h>

namespace keyfreeze {
namespace platform {

bool Win32Api::blockInput(bool enable) {
    // BlockInput 需要管理员权限
    // 注意：Ctrl+Alt+Del 始终可用（系统安全机制）
    return BlockInput(enable) != FALSE;
}

bool Win32Api::isInputBlocked() {
    // Windows 没有直接查询输入锁定状态的 API
    // 需要在应用层维护状态
    return false;
}

bool Win32Api::registerGlobalHotkey(int key, int modifiers, HWND hwnd, int id) {
    return RegisterHotKey(hwnd, id, modifiers, key) != FALSE;
}

void Win32Api::unregisterGlobalHotkey(HWND hwnd, int id) {
    UnregisterHotKey(hwnd, id);
}

bool Win32Api::isRunningAsAdministrator() {
    BOOL is_admin = FALSE;
    PSID admin_group = nullptr;
    
    SID_IDENTIFIER_AUTHORITY nt_authority = SECURITY_NT_AUTHORITY;
    if (AllocateAndInitializeSid(
            &nt_authority,
            2,
            SECURITY_BUILTIN_DOMAIN_RID,
            DOMAIN_ALIAS_RID_ADMINS,
            0, 0, 0, 0, 0, 0,
            &admin_group)) {
        CheckTokenMembership(nullptr, admin_group, &is_admin);
        FreeSid(admin_group);
    }
    
    return is_admin != FALSE;
}

bool Win32Api::requestAdministratorPrivileges() {
    SHELLEXECUTEINFO sei = { sizeof(sei) };
    sei.lpVerb = L"runas";
    sei.lpFile = reinterpret_cast<LPCWSTR>(QCoreApplication::applicationFilePath().utf16());
    sei.hwnd = nullptr;
    sei.nShow = SW_NORMAL;
    
    return ShellExecuteEx(&sei) != FALSE;
}

QString Win32Api::getWindowsVersion() {
    if (IsWindows10OrGreater()) {
        return "Windows 10 or later";
    } else if (IsWindows8OrGreater()) {
        return "Windows 8";
    } else if (IsWindows7OrGreater()) {
        return "Windows 7";
    }
    return "Unknown Windows version";
}

bool Win32Api::isSystemLocked() {
    HDESK desktop = OpenInputDesktop(0, FALSE, DESKTOP_READOBJECTS);
    if (desktop == nullptr) {
        return true;  // 无法打开输入桌面，可能已锁定
    }
    CloseDesktop(desktop);
    return false;
}

HHOOK Win32Api::installLowLevelKeyboardHook(HOOKPROC callback) {
    return SetWindowsHookEx(WH_KEYBOARD_LL, callback, nullptr, 0);
}

void Win32Api::uninstallKeyboardHook(HHOOK hook) {
    if (hook != nullptr) {
        UnhookWindowsHookEx(hook);
    }
}

void Win32Api::disableScreensaver(bool disable) {
    if (disable) {
        SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, FALSE, nullptr, 0);
    } else {
        SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, TRUE, nullptr, 0);
    }
}

bool Win32Api::isScreensaverDisabled() {
    BOOL active = FALSE;
    SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, &active, 0);
    return active == FALSE;
}

} // namespace platform
} // namespace keyfreeze
