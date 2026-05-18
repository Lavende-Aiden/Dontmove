#pragma once

#include <QObject>
#include <Windows.h>

namespace keyfreeze {

// 紧急解锁器：通过低级键盘钩子（WH_KEYBOARD_LL）监听 Esc 连按
// 低级钩子在 BlockInput 生效期间仍可收到键盘事件
// 触发条件：在 kEmergencyUnlockTimeWindowMs 毫秒内连按 Esc kEmergencyUnlockKeyCount 次
class EmergencyUnlocker : public QObject {
    Q_OBJECT

public:
    explicit EmergencyUnlocker(QObject* parent = nullptr);
    ~EmergencyUnlocker() override;

    // 锁定时调用 setActive(true)，解锁后调用 setActive(false)
    void setActive(bool active);
    bool isActive() const;

signals:
    void emergencyUnlockTriggered();

private:
    static LRESULT CALLBACK lowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    static EmergencyUnlocker* s_instance;  // 钩子回调访问当前实例

    void onEscPressed();

    HHOOK hook_            = nullptr;
    int   esc_count_       = 0;
    DWORD first_esc_time_  = 0;  // 第一次 Esc 的 GetTickCount() 时间戳
};

} // namespace keyfreeze
