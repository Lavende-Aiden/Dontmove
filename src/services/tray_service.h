#pragma once

#include <QObject>
#include <QString>

class QSystemTrayIcon;
class QMenu;
class QAction;

namespace keyfreeze {

// 系统托盘服务：管理托盘图标、菜单、消息提示
class TrayService : public QObject {
    Q_OBJECT

public:
    explicit TrayService(QObject* parent = nullptr);
    ~TrayService() override;

    // 初始化托盘
    void setup();

    // 更新锁定状态（同步更新菜单文字和托盘提示）
    void updateLockState(bool locked);

    // 显示气泡消息
    void showMessage(const QString& title, const QString& message);

signals:
    void showMainWindowRequested();
    void toggleLockRequested();
    void exitRequested();

private:
    QSystemTrayIcon* tray_icon_ = nullptr;
    QMenu*           tray_menu_ = nullptr;
    QAction*         toggle_action_ = nullptr;
};

} // namespace keyfreeze
