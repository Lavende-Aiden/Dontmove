#pragma once

#include <QObject>
#include <QString>

namespace keyfreeze {

// 全局热键管理器
// 使用 Win32 RegisterHotKey/WM_HOTKEY 实现全局热键
class HotkeyManager : public QObject {
    Q_OBJECT

public:
    explicit HotkeyManager(QObject* parent = nullptr);
    ~HotkeyManager() override;

    // 注册热键，sequence 格式为 Qt 快捷键字符串，如 "Ctrl+Alt+F12"
    bool registerHotkey(const QString& sequence);
    void unregisterHotkey();
    bool isRegistered() const;

signals:
    void hotkeyTriggered();

private:
    bool is_registered_ = false;
    // Win32 热键 ID（全局唯一）
    static constexpr int kHotkeyId = 1;
};

} // namespace keyfreeze
