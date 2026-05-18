#include <QApplication>
#include <QIcon>

#include "core/emergency_unlocker.h"
#include "core/hotkey_manager.h"
#include "core/input_blocker.h"
#include "services/logger.h"
#include "services/settings_manager.h"
#include "services/tray_service.h"
#include "ui/main_window.h"
#include "ui/win11_style.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("Dontmove");
    QApplication::setApplicationVersion("0.1.0");
    QApplication::setOrganizationName("Aiden Team");
    QApplication::setWindowIcon(QIcon(":/icons/app.ico"));

    // 应用 Windows 11 Fluent Design 风格
    keyfreeze::Win11Style::applyStyleSheet();

    // 初始化日志系统
    keyfreeze::Logger::init(keyfreeze::LogLevel::Info);
    KF_LOG_INFO("应用程序启动中");

    // 创建核心服务对象
    keyfreeze::SettingsManager   settings_mgr;
    keyfreeze::InputBlocker      input_blocker;
    keyfreeze::HotkeyManager     hotkey_mgr;
    keyfreeze::TrayService       tray_service;
    keyfreeze::EmergencyUnlocker emergency_unlocker;

    // 紧急解锁：锁定时激活钩子，解锁时停用
    QObject::connect(&input_blocker,      &keyfreeze::InputBlocker::stateChanged,
                     &emergency_unlocker, &keyfreeze::EmergencyUnlocker::setActive);
    QObject::connect(&emergency_unlocker, &keyfreeze::EmergencyUnlocker::emergencyUnlockTriggered,
                     &input_blocker,      &keyfreeze::InputBlocker::disable);

    // 初始化托盘
    tray_service.setup();

    // 创建主窗口，注入依赖
    keyfreeze::MainWindow window(&input_blocker, &tray_service, &settings_mgr);

    // 尝试启用 Win11 Mica 云母背景效果
    keyfreeze::Win11Style::enableMica(&window);

    // 如已开启热键，注册全局热键
    keyfreeze::AppConfig cfg = settings_mgr.loadConfig();
    if (cfg.enableHotkey) {
        hotkey_mgr.registerHotkey(cfg.hotkeySequence);
    }

    window.show();
    KF_LOG_INFO("应用程序启动完成");

    return app.exec();
}
