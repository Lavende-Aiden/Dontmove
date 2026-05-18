#include "emergency_unlocker.h"

#include "common/constants.h"
#include "services/logger.h"

namespace keyfreeze {

EmergencyUnlocker* EmergencyUnlocker::s_instance = nullptr;

EmergencyUnlocker::EmergencyUnlocker(QObject* parent)
    : QObject(parent) {}

EmergencyUnlocker::~EmergencyUnlocker() {
    setActive(false);
}

void EmergencyUnlocker::setActive(bool active) {
    if (active && hook_ == nullptr) {
        s_instance = this;
        esc_count_      = 0;
        first_esc_time_ = 0;
        hook_ = SetWindowsHookEx(WH_KEYBOARD_LL, lowLevelKeyboardProc, nullptr, 0);
        if (hook_) {
            KF_LOG_INFO("紧急解锁监听已启动（连按 Esc 3 次可解锁）");
        } else {
            KF_LOG_ERROR("安装低级键盘钩子失败");
            s_instance = nullptr;
        }
    } else if (!active && hook_ != nullptr) {
        UnhookWindowsHookEx(hook_);
        hook_      = nullptr;
        s_instance = nullptr;
        KF_LOG_INFO("紧急解锁监听已停止");
    }
}

bool EmergencyUnlocker::isActive() const {
    return hook_ != nullptr;
}

void EmergencyUnlocker::onEscPressed() {
    DWORD now = GetTickCount();
    DWORD window = static_cast<DWORD>(constants::kEmergencyUnlockTimeWindowMs);

    if (esc_count_ == 0 || (now - first_esc_time_) > window) {
        // 超时或首次：重置计数
        esc_count_      = 1;
        first_esc_time_ = now;
    } else {
        ++esc_count_;
    }

    if (esc_count_ >= constants::kEmergencyUnlockKeyCount) {
        esc_count_      = 0;
        first_esc_time_ = 0;
        KF_LOG_INFO("紧急解锁触发");
        emit emergencyUnlockTriggered();
    }
}

LRESULT CALLBACK EmergencyUnlocker::lowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && s_instance != nullptr) {
        auto* kbd = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        if (wParam == WM_KEYDOWN && kbd->vkCode == VK_ESCAPE) {
            s_instance->onEscPressed();
        }
    }
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

} // namespace keyfreeze
