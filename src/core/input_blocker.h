#pragma once

#include <QObject>
#include <Windows.h>

class QTimer;

namespace keyfreeze {

// 输入拦截器：支持选择性锁定键盘/鼠标
// - 两者均锁：使用 BlockInput()（需要管理员权限，最安全）
// - 仅键盘：安装 WH_KEYBOARD_LL 钩子吞掉所有按键事件
// - 仅鼠标：安装 WH_MOUSE_LL 钩子吞掉所有鼠标事件
class InputBlocker : public QObject {
    Q_OBJECT

public:
    explicit InputBlocker(QObject* parent = nullptr);
    ~InputBlocker() override;

    // 启用输入阻断
    // lockKeyboard / lockMouse：指定锁定哪个设备
    // autoUnlockMinutes：0 表示不自动解锁
    bool enable(bool lockKeyboard = true, bool lockMouse = true, int autoUnlockMinutes = 0);
    void disable();
    bool isEnabled() const;
    int  remainingSeconds() const;

signals:
    void stateChanged(bool enabled);
    // 每秒发出剩余秒数；-1 表示无计时器
    void timeRemainingChanged(int seconds);

private slots:
    void onAutoUnlockTimeout();
    void onCountdownTick();

private:
    // 低级键盘钩子回调（用于仅锁键盘模式）
    static LRESULT CALLBACK keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam);
    // 低级鼠标钩子回调（用于仅锁鼠标模式）
    static LRESULT CALLBACK mouseHookProc(int nCode, WPARAM wParam, LPARAM lParam);
    static InputBlocker* s_instance;

    bool    is_enabled_        = false;
    bool    use_block_input_   = false;  // 是否使用 BlockInput（两者同锁时）
    bool    lock_keyboard_     = false;
    bool    lock_mouse_        = false;
    HHOOK   kb_hook_           = nullptr;
    HHOOK   mouse_hook_        = nullptr;
    int     remaining_seconds_ = 0;
    QTimer* auto_unlock_timer_ = nullptr;
    QTimer* countdown_timer_   = nullptr;
};

} // namespace keyfreeze
