#pragma once

#include "common/types.h"

#include <QObject>
#include <QSettings>
#include <memory>

namespace keyfreeze {

class SettingsManager : public QObject {
    Q_OBJECT

public:
    explicit SettingsManager(QObject* parent = nullptr);
    
    // 加载和保存配置
    AppConfig loadConfig();
    void saveConfig(const AppConfig& config);
    void resetToDefaults();
    
    // 便捷访问器
    bool isKeyboardLockEnabled() const;
    bool isMouseLockEnabled() const;
    bool isHotkeyEnabled() const;
    QString getHotkeySequence() const;
    int getAutoUnlockMinutes() const;
    
    // 信号
signals:
    void configChanged(const AppConfig& newConfig);

private:
    std::unique_ptr<QSettings> settings_;
    AppConfig current_config_;
    
    void loadFromSettings();
    void saveToSettings();
};

} // namespace keyfreeze
