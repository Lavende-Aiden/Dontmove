#include "input_blocker.h"

#include "platform/win32_api.h"
#include "services/logger.h"

#include <QTimer>

namespace keyfreeze {

InputBlocker* InputBlocker::s_instance = nullptr;

// ---------------------------------------------------------------------------
// 钩子回调：吃掉所有键盘事件
// ---------------------------------------------------------------------------
LRESULT CALLBACK InputBlocker::keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    // 先让后续钩子（如 EmergencyUnlocker）处理事件，再吞掉它
    CallNextHookEx(nullptr, nCode, wParam, lParam);
    if (nCode == HC_ACTION && s_instance && s_instance->lock_keyboard_) {
        return 1;  // 吞掉事件，阻止传递到应用程序
    }
    return 0;
}

// 钩子回调：吃掉所有鼠标事件
LRESULT CALLBACK InputBlocker::mouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    CallNextHookEx(nullptr, nCode, wParam, lParam);
    if (nCode == HC_ACTION && s_instance && s_instance->lock_mouse_) {
        return 1;  // 吞掉事件
    }
    return 0;
}

// ---------------------------------------------------------------------------

InputBlocker::InputBlocker(QObject* parent)
    : QObject(parent),
      auto_unlock_timer_(new QTimer(this)),
      countdown_timer_(new QTimer(this)) {
    auto_unlock_timer_->setSingleShot(true);
    countdown_timer_->setInterval(1000);
    connect(auto_unlock_timer_, &QTimer::timeout, this, &InputBlocker::onAutoUnlockTimeout);
    connect(countdown_timer_,   &QTimer::timeout, this, &InputBlocker::onCountdownTick);
}

InputBlocker::~InputBlocker() {
    if (is_enabled_) {
        disable();
    }
}

bool InputBlocker::enable(bool lockKeyboard, bool lockMouse, int autoUnlockMinutes) {
    if (is_enabled_) return true;
    if (!lockKeyboard && !lockMouse) {
        KF_LOG_WARNING("未选择任何锁定设备，跳过锁定");
        return false;
    }

    lock_keyboard_   = lockKeyboard;
    lock_mouse_      = lockMouse;
    use_block_input_ = lockKeyboard && lockMouse;

    if (use_block_input_) {
        // 两者同时锁定：使用 BlockInput（需要管理员权限）
        if (!platform::Win32Api::isRunningAsAdministrator()) {
            KF_LOG_WARNING("同时锁定键盘+鼠标需要管理员权限，请以管理员身份运行");
            return false;
        }
        if (!platform::Win32Api::blockInput(true)) {
            KF_LOG_ERROR("调用 BlockInput 失败");
            return false;
        }
        KF_LOG_INFO("输入阻断已启用（键盘 + 鼠标， BlockInput）");
    } else {
        // 单独锁定键盘或鼠标：使用低级钩子（无需管理员权限）
        s_instance = this;
        if (lockKeyboard) {
            kb_hook_ = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHookProc, nullptr, 0);
            if (!kb_hook_) {
                KF_LOG_ERROR("安装键盘钩子失败");
                s_instance = nullptr;
                return false;
            }
            KF_LOG_INFO("输入阻断已启用（仅键盘， WH_KEYBOARD_LL）");
        }
        if (lockMouse) {
            mouse_hook_ = SetWindowsHookEx(WH_MOUSE_LL, mouseHookProc, nullptr, 0);
            if (!mouse_hook_) {
                KF_LOG_ERROR("安装鼠标钩子失败");
                if (kb_hook_) { UnhookWindowsHookEx(kb_hook_); kb_hook_ = nullptr; }
                s_instance = nullptr;
                return false;
            }
            KF_LOG_INFO("输入阻断已启用（仅鼠标， WH_MOUSE_LL）");
        }
    }

    is_enabled_ = true;

    // 自动解锁计时器
    if (autoUnlockMinutes > 0) {
        remaining_seconds_ = autoUnlockMinutes * 60;
        auto_unlock_timer_->start(autoUnlockMinutes * 60 * 1000);
        countdown_timer_->start();
        emit timeRemainingChanged(remaining_seconds_);
        KF_LOG_INFO(QString("%1 分钟后自动解锁").arg(autoUnlockMinutes));
    } else {
        remaining_seconds_ = 0;
        emit timeRemainingChanged(-1);
    }

    emit stateChanged(true);
    return true;
}

void InputBlocker::disable() {
    if (!is_enabled_) return;

    auto_unlock_timer_->stop();
    countdown_timer_->stop();
    remaining_seconds_ = 0;

    if (use_block_input_) {
        platform::Win32Api::blockInput(false);
    } else {
        if (kb_hook_)    { UnhookWindowsHookEx(kb_hook_);    kb_hook_    = nullptr; }
        if (mouse_hook_) { UnhookWindowsHookEx(mouse_hook_); mouse_hook_ = nullptr; }
        if (s_instance == this) s_instance = nullptr;
    }

    lock_keyboard_   = false;
    lock_mouse_      = false;
    use_block_input_ = false;
    is_enabled_      = false;

    emit stateChanged(false);
    emit timeRemainingChanged(-1);
    KF_LOG_INFO("输入阻断已关闭");
}

bool InputBlocker::isEnabled() const {
    return is_enabled_;
}

int InputBlocker::remainingSeconds() const {
    return remaining_seconds_;
}

void InputBlocker::onAutoUnlockTimeout() {
    KF_LOG_INFO("自动解锁计时器触发");
    disable();
}

void InputBlocker::onCountdownTick() {
    if (remaining_seconds_ > 0) {
        --remaining_seconds_;
        emit timeRemainingChanged(remaining_seconds_);
    }
}

} // namespace keyfreeze
