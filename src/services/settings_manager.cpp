#include "settings_manager.h"

#include "common/constants.h"

#include <QCoreApplication>
#include <QDir>

namespace keyfreeze {

SettingsManager::SettingsManager(QObject* parent)
    : QObject(parent) {
    // 使用 INI 文件而不是注册表，符合绿色软件标准
    QString configPath = QCoreApplication::applicationDirPath() + "/config.ini";
    settings_ = std::make_unique<QSettings>(configPath, QSettings::IniFormat);
    loadFromSettings();
}

AppConfig SettingsManager::loadConfig() {
    loadFromSettings();
    return current_config_;
}

void SettingsManager::saveConfig(const AppConfig& config) {
    current_config_ = config;
    saveToSettings();
    emit configChanged(config);
}

void SettingsManager::resetToDefaults() {
    current_config_ = AppConfig{};
    saveToSettings();
    emit configChanged(current_config_);
}

bool SettingsManager::isKeyboardLockEnabled() const {
    return current_config_.lockKeyboard;
}

bool SettingsManager::isMouseLockEnabled() const {
    return current_config_.lockMouse;
}

bool SettingsManager::isHotkeyEnabled() const {
    return current_config_.enableHotkey;
}

QString SettingsManager::getHotkeySequence() const {
    return current_config_.hotkeySequence;
}

int SettingsManager::getAutoUnlockMinutes() const {
    return current_config_.autoUnlockMinutes;
}

void SettingsManager::loadFromSettings() {
    current_config_.lockKeyboard = settings_->value("lockKeyboard", true).toBool();
    current_config_.lockMouse = settings_->value("lockMouse", true).toBool();
    current_config_.enableHotkey = settings_->value("enableHotkey", false).toBool();
    current_config_.hotkeySequence = settings_->value("hotkeySequence", constants::kDefaultHotkey).toString();
    current_config_.autoUnlockMinutes = settings_->value("autoUnlockMinutes", constants::kDefaultAutoUnlockMinutes).toInt();
    current_config_.startMinimized = settings_->value("startMinimized", false).toBool();
}

void SettingsManager::saveToSettings() {
    settings_->setValue("lockKeyboard", current_config_.lockKeyboard);
    settings_->setValue("lockMouse", current_config_.lockMouse);
    settings_->setValue("enableHotkey", current_config_.enableHotkey);
    settings_->setValue("hotkeySequence", current_config_.hotkeySequence);
    settings_->setValue("autoUnlockMinutes", current_config_.autoUnlockMinutes);
    settings_->setValue("startMinimized", current_config_.startMinimized);
    settings_->sync();
}

} // namespace keyfreeze
