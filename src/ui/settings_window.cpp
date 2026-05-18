#include "settings_window.h"

#include "services/settings_manager.h"

#include <QCheckBox>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>

namespace keyfreeze {

SettingsWindow::SettingsWindow(QWidget* parent)
    : QWidget(parent) {
    setupUi();
}

void SettingsWindow::setupUi() {
    setWindowTitle(u8"设置");
    setWindowFlags(windowFlags() | Qt::Window);
    setObjectName("settingsWindowRoot");
    resize(440, 380);

    auto* main_layout = new QVBoxLayout(this);
    main_layout->setContentsMargins(28, 24, 28, 20);
    main_layout->setSpacing(20);

    // ---- 锁定选项组 ----
    auto* lock_group  = new QGroupBox(u8"锁定选项", this);
    lock_group->setObjectName("lockGroup");
    auto* lock_layout = new QVBoxLayout(lock_group);
    lock_layout->setSpacing(12);

    keyboard_lock_checkbox_ = new QCheckBox(u8"锁定键盘", this);
    keyboard_lock_checkbox_->setChecked(true);
    mouse_lock_checkbox_    = new QCheckBox(u8"锁定鼠标", this);
    mouse_lock_checkbox_->setChecked(true);
    hotkey_checkbox_        = new QCheckBox(u8"启用全局热键 (Ctrl+Alt+F12)", this);

    lock_layout->addWidget(keyboard_lock_checkbox_);
    lock_layout->addWidget(mouse_lock_checkbox_);
    lock_layout->addWidget(hotkey_checkbox_);

    // ---- 自动解锁选项 ----
    auto* timer_group  = new QGroupBox(u8"自动解锁", this);
    timer_group->setObjectName("timerGroup");
    auto* timer_layout = new QFormLayout(timer_group);
    timer_layout->setSpacing(12);
    timer_layout->setContentsMargins(20, 24, 20, 16);

    auto_unlock_spinbox_ = new QSpinBox(this);
    auto_unlock_spinbox_->setRange(1, 240);
    auto_unlock_spinbox_->setValue(30);
    auto_unlock_spinbox_->setSuffix(u8" 分钟");
    timer_layout->addRow(u8"分钟后自动解锁：", auto_unlock_spinbox_);

    // ---- 按钮行 ----
    auto* btn_layout = new QHBoxLayout();
    btn_layout->setSpacing(12);
    auto* apply_btn  = new QPushButton(u8"应用", this);
    apply_btn->setObjectName("applyButton");
    auto* reset_btn  = new QPushButton(u8"重置", this);
    reset_btn->setObjectName("resetButton");
    btn_layout->addStretch();
    btn_layout->addWidget(apply_btn);
    btn_layout->addWidget(reset_btn);

    connect(apply_btn, &QPushButton::clicked, this, &SettingsWindow::onApplyClicked);
    connect(reset_btn, &QPushButton::clicked, this, &SettingsWindow::onResetClicked);

    main_layout->addWidget(lock_group);
    main_layout->addWidget(timer_group);
    main_layout->addStretch();
    main_layout->addLayout(btn_layout);
}

void SettingsWindow::setSettingsManager(SettingsManager* mgr) {
    settings_manager_ = mgr;
    if (settings_manager_) {
        loadFromManager();
    }
}

void SettingsWindow::loadFromManager() {
    if (!settings_manager_) return;
    AppConfig cfg = settings_manager_->loadConfig();
    keyboard_lock_checkbox_->setChecked(cfg.lockKeyboard);
    mouse_lock_checkbox_->setChecked(cfg.lockMouse);
    hotkey_checkbox_->setChecked(cfg.enableHotkey);
    auto_unlock_spinbox_->setValue(cfg.autoUnlockMinutes);
}

void SettingsWindow::saveToManager() {
    if (!settings_manager_) return;
    AppConfig cfg;
    cfg.lockKeyboard      = keyboard_lock_checkbox_->isChecked();
    cfg.lockMouse         = mouse_lock_checkbox_->isChecked();
    cfg.enableHotkey      = hotkey_checkbox_->isChecked();
    cfg.autoUnlockMinutes = auto_unlock_spinbox_->value();
    settings_manager_->saveConfig(cfg);
}

void SettingsWindow::onApplyClicked() {
    saveToManager();
    hide();
}

void SettingsWindow::onResetClicked() {
    if (settings_manager_) {
        settings_manager_->resetToDefaults();
        loadFromManager();
    }
}

} // namespace keyfreeze
