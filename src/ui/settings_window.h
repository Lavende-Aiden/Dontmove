#pragma once

#include "common/types.h"

#include <QWidget>

class QCheckBox;
class QSpinBox;

namespace keyfreeze {

class SettingsManager;

class SettingsWindow : public QWidget {
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget* parent = nullptr);

    // 绑定配置管理器，继路应在创建后第一时间设置
    void setSettingsManager(SettingsManager* mgr);

private slots:
    void onApplyClicked();
    void onResetClicked();

private:
    void setupUi();
    void loadFromManager();
    void saveToManager();

    SettingsManager* settings_manager_     = nullptr;

    QCheckBox* keyboard_lock_checkbox_     = nullptr;
    QCheckBox* mouse_lock_checkbox_        = nullptr;
    QCheckBox* hotkey_checkbox_            = nullptr;
    QSpinBox*  auto_unlock_spinbox_        = nullptr;
};

} // namespace keyfreeze
