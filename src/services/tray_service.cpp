#include "tray_service.h"

#include "services/logger.h"

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QIcon>

namespace keyfreeze {

TrayService::TrayService(QObject* parent)
    : QObject(parent) {}

TrayService::~TrayService() {
    delete tray_menu_;
}

void TrayService::setup() {
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        KF_LOG_WARNING("系统托盘不可用");
        return;
    }

    // 创建托盘图标
    tray_icon_ = new QSystemTrayIcon(this);
    tray_icon_->setIcon(QIcon(":/icons/app.ico"));
    tray_icon_->setToolTip("Dontmove - 输入保护");

    // 创建右键菜单
    tray_menu_ = new QMenu();
    auto* show_action   = tray_menu_->addAction("显示主窗口");
    toggle_action_      = tray_menu_->addAction("启用锁定");
    tray_menu_->addSeparator();
    auto* exit_action   = tray_menu_->addAction("退出");

    connect(show_action,   &QAction::triggered, this, &TrayService::showMainWindowRequested);
    connect(toggle_action_, &QAction::triggered, this, &TrayService::toggleLockRequested);
    connect(exit_action,   &QAction::triggered, this, &TrayService::exitRequested);

    tray_icon_->setContextMenu(tray_menu_);
    tray_icon_->show();

    // 双击托盘图标 -> 显示主窗口
    connect(tray_icon_, &QSystemTrayIcon::activated,
            this, [this](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::DoubleClick) {
            emit showMainWindowRequested();
        }
    });

    KF_LOG_INFO("托盘服务已初始化");
}

void TrayService::updateLockState(bool locked) {
    if (toggle_action_) {
        toggle_action_->setText(locked ? "关闭锁定" : "启用锁定");
    }
    if (tray_icon_) {
        tray_icon_->setIcon(QIcon(locked ? ":/icons/lock.ico" : ":/icons/app.ico"));
        tray_icon_->setToolTip(locked ? "Dontmove - 输入已锁定" : "Dontmove - 输入保护");
    }
}

void TrayService::showMessage(const QString& title, const QString& message) {
    if (tray_icon_) {
        tray_icon_->showMessage(title, message, QSystemTrayIcon::Information, 3000);
    }
}

} // namespace keyfreeze
