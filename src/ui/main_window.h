#pragma once

#include <QMainWindow>
#include <QPoint>

class QLabel;
class QPushButton;
class QMouseEvent;
class QShowEvent;
class QGraphicsOpacityEffect;

namespace keyfreeze {

class InputBlocker;
class SettingsWindow;
class SettingsManager;
class TrayService;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(InputBlocker*    blocker,
                        TrayService*     tray,
                        SettingsManager* settings,
                        QWidget*         parent = nullptr);

public slots:
    void onLockStateChanged(bool locked);
    void onTimeRemainingChanged(int seconds);

private slots:
    void onToggleClicked();
    void onOpenSettingsClicked();
    void onTopClicked();
    void onMinimizeClicked();
    void onCloseClicked();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void showEvent(QShowEvent* event) override;

private:
    void setupUi();
    void setupConnections();
    void startSplashAnimation();

    InputBlocker*    input_blocker_   = nullptr;
    TrayService*     tray_service_    = nullptr;
    SettingsManager* settings_mgr_    = nullptr;

    QWidget*         title_bar_       = nullptr;
    QWidget*         content_widget_  = nullptr;
    QLabel*          title_label_     = nullptr;
    QLabel*          status_label_    = nullptr;
    QLabel*          timer_label_     = nullptr;
    QPushButton*     toggle_button_   = nullptr;
    QPushButton*     settings_button_ = nullptr;
    QPushButton*     top_button_      = nullptr;
    QPushButton*     minimize_button_ = nullptr;
    QPushButton*     close_button_    = nullptr;
    QLabel*          hint_label_      = nullptr;
    SettingsWindow*  settings_window_ = nullptr;

    // 启动动画
    QLabel*                splash_label_    = nullptr;
    QGraphicsOpacityEffect* splash_opacity_ = nullptr;
    QGraphicsOpacityEffect* content_opacity_ = nullptr;
    QGraphicsOpacityEffect* title_bar_opacity_ = nullptr;
    bool                   splash_played_   = false;

    QPoint           drag_position_;
    bool             is_dragging_     = false;
    bool             is_pinned_       = false;
};

} // namespace keyfreeze
