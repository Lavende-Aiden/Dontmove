#include "hotkey_manager.h"

#include "platform/win32_api.h"
#include "services/logger.h"

#include <QKeySequence>
#include <Windows.h>

namespace keyfreeze {

HotkeyManager::HotkeyManager(QObject* parent)
    : QObject(parent) {}

HotkeyManager::~HotkeyManager() {
    unregisterHotkey();
}

bool HotkeyManager::registerHotkey(const QString& sequence) {
    if (is_registered_) {
        unregisterHotkey();
    }

    QKeySequence keySeq(sequence);
    if (keySeq.isEmpty()) {
        KF_LOG_WARNING("无效的热键序列: " + sequence);
        return false;
    }

    // 解析 Qt 快捷键到 Win32 修饰符和虚拟键码
    int qt_key = keySeq[0].key();
    int modifiers = 0;
    Qt::KeyboardModifiers qt_mods = keySeq[0].keyboardModifiers();
    if (qt_mods & Qt::ControlModifier) modifiers |= MOD_CONTROL;
    if (qt_mods & Qt::AltModifier)     modifiers |= MOD_ALT;
    if (qt_mods & Qt::ShiftModifier)   modifiers |= MOD_SHIFT;
    if (qt_mods & Qt::MetaModifier)    modifiers |= MOD_WIN;

    // 将 Qt 虚拟键码映射到 Win32 VK
    UINT vk = static_cast<UINT>(qt_key & ~Qt::KeyboardModifierMask);

    // 使用线程消息队列注册（HWND=NULL），需在消息循环中处理 WM_HOTKEY
    if (platform::Win32Api::registerGlobalHotkey(static_cast<int>(vk), modifiers, nullptr, kHotkeyId)) {
        is_registered_ = true;
        KF_LOG_INFO("热键注册成功: " + sequence);
        return true;
    }

    KF_LOG_WARNING("热键注册失败（可能已被其他程序占用）: " + sequence);
    return false;
}

void HotkeyManager::unregisterHotkey() {
    if (!is_registered_) return;

    platform::Win32Api::unregisterGlobalHotkey(nullptr, kHotkeyId);
    is_registered_ = false;
    KF_LOG_INFO("热键已注销");
}

bool HotkeyManager::isRegistered() const {
    return is_registered_;
}

} // namespace keyfreeze
