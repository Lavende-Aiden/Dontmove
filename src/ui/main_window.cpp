#include "main_window.h"

#include "settings_window.h"
#include "common/types.h"
#include "core/input_blocker.h"
#include "services/settings_manager.h"
#include "services/tray_service.h"

#include <QApplication>
#include <QFrame>
#include <QGraphicsOpacityEffect>
#include <QIcon>
#include <QLabel>
#include <QMouseEvent>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QSequentialAnimationGroup>
#include <QShowEvent>
#include <QStyle>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

namespace keyfreeze {

MainWindow::MainWindow(InputBlocker* blocker, TrayService* tray, SettingsManager* settings, QWidget* parent)
    : QMainWindow(parent),
      input_blocker_(blocker),
      tray_service_(tray),
      settings_mgr_(settings) {
    // 移除默认标题栏，保留系统任务栏交互
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    setupUi();
    setupConnections();
    if (settings_mgr_) {
        settings_window_->setSettingsManager(settings_mgr_);
    }
}

void MainWindow::setupUi() {
    setWindowTitle("Dontmove");
    setWindowIcon(QIcon(":/icons/app.ico"));
    resize(480, 440);

    // ---- 中央部件 ----
    auto* central = new QWidget(this);
    central->setObjectName("centralWidget");
    auto* main_layout = new QVBoxLayout(central);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(0);

    // ---- 自定义标题栏 ----
    title_bar_ = new QWidget(this);
    title_bar_->setObjectName("titleBar");
    title_bar_->setFixedHeight(36);
    auto* title_bar_layout = new QHBoxLayout(title_bar_);
    title_bar_layout->setContentsMargins(12, 0, 0, 0);
    title_bar_layout->setSpacing(0);

    // 标题栏左侧：应用图标 + 名称
    auto* title_bar_icon = new QLabel(this);
    title_bar_icon->setObjectName("titleBarIcon");
    title_bar_icon->setPixmap(QIcon(":/icons/app.ico").pixmap(16, 16));
    auto* title_bar_label = new QLabel("Dontmove", this);
    title_bar_label->setObjectName("titleBarLabel");

    // 置顶按钮
    top_button_ = new QPushButton(this);
    top_button_->setObjectName("topButton");
    top_button_->setIcon(QIcon(":/icons/up.ico"));
    top_button_->setCursor(Qt::PointingHandCursor);
    top_button_->setToolTip(u8"保持置顶");

    // 最小化按钮
    minimize_button_ = new QPushButton(this);
    minimize_button_->setObjectName("minimizeButton");
    minimize_button_->setIcon(QIcon(":/icons/smaller.ico"));
    minimize_button_->setCursor(Qt::PointingHandCursor);
    minimize_button_->setToolTip(u8"最小化");

    // 关闭按钮
    close_button_ = new QPushButton(this);
    close_button_->setObjectName("closeButton");
    close_button_->setIcon(QIcon(":/icons/close.ico"));
    close_button_->setCursor(Qt::PointingHandCursor);
    close_button_->setToolTip(u8"关闭");

    title_bar_layout->addWidget(title_bar_icon);
    title_bar_layout->addWidget(title_bar_label);
    title_bar_layout->addStretch();
    title_bar_layout->addWidget(top_button_);
    title_bar_layout->addWidget(minimize_button_);
    title_bar_layout->addWidget(close_button_);

    // ---- 内容区域 ----
    content_widget_ = new QWidget(this);
    content_widget_->setObjectName("contentWidget");
    auto* content_layout = new QVBoxLayout(content_widget_);
    content_layout->setContentsMargins(40, 24, 40, 24);
    content_layout->setSpacing(0);

    // ---- 标题 ----
    title_label_ = new QLabel("Dontmove", this);
    title_label_->setObjectName("titleLabel");
    title_label_->setAlignment(Qt::AlignCenter);

    // ---- 状态 ----
    status_label_ = new QLabel("\347\212\266\346\200\201\357\274\232\346\234\252\351\224\201\345\256\232", this);
    status_label_->setObjectName("statusLabel");
    status_label_->setAlignment(Qt::AlignCenter);

    // ---- 倒计时 ----
    timer_label_ = new QLabel("", this);
    timer_label_->setObjectName("timerLabel");
    timer_label_->setAlignment(Qt::AlignCenter);
    timer_label_->setVisible(false);

    // ---- 分隔线 ----
    auto* separator = new QFrame(this);
    separator->setObjectName("separator");
    separator->setFrameShape(QFrame::HLine);
    separator->setFixedHeight(1);

    // ---- 主按钮（启用/解除锁定） ----
    toggle_button_ = new QPushButton("\345\220\257\347\224\250\351\224\201\345\256\232", this);
    toggle_button_->setObjectName("toggleButton");
    toggle_button_->setCursor(Qt::PointingHandCursor);

    // ---- 设置按钮（图标，右下角） ----
    settings_button_ = new QPushButton(this);
    settings_button_->setObjectName("settingsButton");
    settings_button_->setIcon(QIcon(":/icons/setting.ico"));
    settings_button_->setCursor(Qt::PointingHandCursor);
    settings_button_->setToolTip(u8"设置");

    // ---- 底部提示 ----
    hint_label_ = new QLabel(u8"连按 Esc × 3 紧急解锁", this);
    hint_label_->setObjectName("hintLabel");
    hint_label_->setAlignment(Qt::AlignCenter);

    // ---- 设置窗口 ----
    settings_window_ = new SettingsWindow(this);

    // ---- 底部行：提示 + 设置图标 ----
    auto* bottom_layout = new QHBoxLayout();
    bottom_layout->setContentsMargins(0, 0, 0, 0);
    bottom_layout->addWidget(hint_label_);
    bottom_layout->addStretch();
    bottom_layout->addWidget(settings_button_);

    // ---- 内容布局 ----
    content_layout->addWidget(title_label_);
    content_layout->addSpacing(12);
    content_layout->addWidget(status_label_);
    content_layout->addWidget(timer_label_);
    content_layout->addSpacing(16);
    content_layout->addWidget(separator);
    content_layout->addSpacing(24);
    content_layout->addWidget(toggle_button_, 0, Qt::AlignHCenter);
    content_layout->addStretch();
    content_layout->addSpacing(8);
    content_layout->addLayout(bottom_layout);

    // ---- 主布局 ----
    main_layout->addWidget(title_bar_);
    main_layout->addWidget(content_widget_);

    // ---- 启动动画：闪屏标签 ----
    splash_label_ = new QLabel("Dontmove", central);
    splash_label_->setObjectName("splashLabel");
    splash_label_->setAlignment(Qt::AlignCenter);
    splash_label_->raise();  // 置于最上层

    // 透明度效果
    splash_opacity_ = new QGraphicsOpacityEffect(splash_label_);
    splash_opacity_->setOpacity(0.0);
    splash_label_->setGraphicsEffect(splash_opacity_);

    content_opacity_ = new QGraphicsOpacityEffect(content_widget_);
    content_opacity_->setOpacity(0.0);
    content_widget_->setGraphicsEffect(content_opacity_);

    title_bar_opacity_ = new QGraphicsOpacityEffect(title_bar_);
    title_bar_opacity_->setOpacity(0.0);
    title_bar_->setGraphicsEffect(title_bar_opacity_);

    setCentralWidget(central);
}

void MainWindow::setupConnections() {
    connect(top_button_,      &QPushButton::clicked, this, &MainWindow::onTopClicked);
    connect(minimize_button_, &QPushButton::clicked, this, &MainWindow::onMinimizeClicked);
    connect(close_button_,    &QPushButton::clicked, this, &MainWindow::onCloseClicked);
    connect(toggle_button_,   &QPushButton::clicked, this, &MainWindow::onToggleClicked);
    connect(settings_button_, &QPushButton::clicked, this, &MainWindow::onOpenSettingsClicked);

    if (input_blocker_) {
        connect(input_blocker_, &InputBlocker::stateChanged,
                this, &MainWindow::onLockStateChanged);
        connect(input_blocker_, &InputBlocker::timeRemainingChanged,
                this, &MainWindow::onTimeRemainingChanged);
    }

    if (tray_service_) {
        connect(tray_service_, &TrayService::showMainWindowRequested,
                this, &QWidget::show);
        connect(tray_service_, &TrayService::toggleLockRequested,
                this, &MainWindow::onToggleClicked);
        connect(tray_service_, &TrayService::exitRequested,
                qApp, &QApplication::quit);
    }
}

void MainWindow::onToggleClicked() {
    if (!input_blocker_) return;

    if (input_blocker_->isEnabled()) {
        input_blocker_->disable();
    } else {
        bool lock_kb = true;
        bool lock_ms = true;
        int  auto_unlock = 0;
        if (settings_mgr_) {
            AppConfig cfg = settings_mgr_->loadConfig();
            lock_kb      = cfg.lockKeyboard;
            lock_ms      = cfg.lockMouse;
            auto_unlock  = cfg.autoUnlockMinutes;
        }
        if (!input_blocker_->enable(lock_kb, lock_ms, auto_unlock)) {
            status_label_->setText("\347\212\266\346\200\201\357\274\232\351\224\201\345\256\232\345\244\261\350\264\245\357\274\210\345\220\214\346\227\266\351\224\201\345\256\232\351\224\256\351\274\240\351\234\200\347\256\241\347\220\206\345\221\230\346\235\203\351\231\220\357\274\231");
        }
    }
}

void MainWindow::onLockStateChanged(bool locked) {
    // 更新状态标签
    status_label_->setText(locked ? "\347\212\266\346\200\201\357\274\232\345\267\262\351\224\201\345\256\232" : "\347\212\266\346\200\201\357\274\232\346\234\252\351\224\201\345\256\232");
    status_label_->setObjectName(locked ? "lockedStatusLabel" : "statusLabel");
    // 强制刷新 QSS（objectName 变化需要重新 polish）
    status_label_->style()->unpolish(status_label_);
    status_label_->style()->polish(status_label_);

    // 更新切换按钮
    toggle_button_->setText(locked ? "\350\247\243\351\231\244\351\224\201\345\256\232" : "\345\220\257\347\224\250\351\224\201\345\256\232");
    toggle_button_->setProperty("locked", locked);
    toggle_button_->style()->unpolish(toggle_button_);
    toggle_button_->style()->polish(toggle_button_);

    if (!locked) {
        timer_label_->setVisible(false);
    }

    if (tray_service_) {
        tray_service_->updateLockState(locked);
        tray_service_->showMessage("Dontmove",
            locked ? "\350\276\223\345\205\245\345\267\262\351\224\201\345\256\232" : "\350\276\223\345\205\245\345\267\262\350\247\243\351\231\244\351\224\201\345\256\232");
    }
}

void MainWindow::onTimeRemainingChanged(int seconds) {
    if (seconds < 0) {
        timer_label_->setVisible(false);
        return;
    }
    int m = seconds / 60;
    int s = seconds % 60;
    timer_label_->setText(QString("\350\207\252\345\212\250\350\247\243\351\224\201\357\274\232%1:%2")
                              .arg(m, 2, 10, QChar('0'))
                              .arg(s, 2, 10, QChar('0')));
    timer_label_->setVisible(true);
}

void MainWindow::onTopClicked() {
    if (is_pinned_) {
#ifdef Q_OS_WIN
        HWND hwnd = reinterpret_cast<HWND>(winId());
        SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
#else
        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
        show();
#endif
    } else {
#ifdef Q_OS_WIN
        HWND hwnd = reinterpret_cast<HWND>(winId());
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
#else
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
        show();
#endif
    }

    is_pinned_ = !is_pinned_;
    top_button_->setProperty("pinned", is_pinned_);
    top_button_->style()->unpolish(top_button_);
    top_button_->style()->polish(top_button_);
    top_button_->setToolTip(is_pinned_ ? u8"\345\217\226\346\266\210\347\275\256\351\241\266" : u8"\344\277\235\346\214\201\347\275\256\351\241\266");
}

void MainWindow::onMinimizeClicked() {
    showMinimized();
}

void MainWindow::onCloseClicked() {
    close();
}

void MainWindow::onOpenSettingsClicked() {
    settings_window_->show();
    settings_window_->raise();
    settings_window_->activateWindow();
}

// ---- 鼠标拖拽标题栏移动窗口 ----
void MainWindow::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && title_bar_) {
        // 检查点击是否在标题栏区域内
        QPoint pos = event->position().toPoint();
        if (pos.y() <= title_bar_->height()) {
            is_dragging_ = true;
            drag_position_ = event->globalPosition().toPoint() - geometry().topLeft();
            event->accept();
            return;
        }
    }
    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    if (is_dragging_ && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPosition().toPoint() - drag_position_);
        event->accept();
        return;
    }
    QMainWindow::mouseMoveEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event) {
    if (is_dragging_) {
        is_dragging_ = false;
        event->accept();
        return;
    }
    QMainWindow::mouseReleaseEvent(event);
}

// ---- 启动动画 ----
void MainWindow::showEvent(QShowEvent* event) {
    QMainWindow::showEvent(event);

    if (!splash_played_) {
        splash_played_ = true;

        // 定位闪屏标签到窗口中央
        int w = centralWidget()->width();
        int h = centralWidget()->height();
        int label_h = 60;
        splash_label_->setGeometry(0, (h - label_h) / 2, w, label_h);

        startSplashAnimation();
    }
}

void MainWindow::startSplashAnimation() {
    int w = centralWidget()->width();
    int h = centralWidget()->height();

    // ---- Phase 1: 闪屏文字淡入 (800ms) ----
    auto* fade_in = new QPropertyAnimation(splash_opacity_, "opacity");
    fade_in->setDuration(800);
    fade_in->setStartValue(0.0);
    fade_in->setEndValue(1.0);
    fade_in->setEasingCurve(QEasingCurve::InOutQuad);

    // ---- Phase 2: 闪屏文字上移到标题位置 (500ms) ----
    QRect start_geo = splash_label_->geometry();
    int target_y = 36 + 24;  // title_bar(36) + content top margin(24)
    QRect end_geo(0, target_y, w, 60);

    auto* move_up = new QPropertyAnimation(splash_label_, "geometry");
    move_up->setDuration(500);
    move_up->setStartValue(start_geo);
    move_up->setEndValue(end_geo);
    move_up->setEasingCurve(QEasingCurve::OutCubic);

    // ---- Phase 3: 内容淡入 + 闪屏淡出 (并行, 400ms) ----
    auto* content_fade = new QPropertyAnimation(content_opacity_, "opacity");
    content_fade->setDuration(400);
    content_fade->setStartValue(0.0);
    content_fade->setEndValue(1.0);
    content_fade->setEasingCurve(QEasingCurve::InOutQuad);

    auto* title_bar_fade = new QPropertyAnimation(title_bar_opacity_, "opacity");
    title_bar_fade->setDuration(400);
    title_bar_fade->setStartValue(0.0);
    title_bar_fade->setEndValue(1.0);
    title_bar_fade->setEasingCurve(QEasingCurve::InOutQuad);

    auto* splash_fade_out = new QPropertyAnimation(splash_opacity_, "opacity");
    splash_fade_out->setDuration(400);
    splash_fade_out->setStartValue(1.0);
    splash_fade_out->setEndValue(0.0);
    splash_fade_out->setEasingCurve(QEasingCurve::InOutQuad);

    auto* phase3 = new QParallelAnimationGroup;
    phase3->addAnimation(content_fade);
    phase3->addAnimation(title_bar_fade);
    phase3->addAnimation(splash_fade_out);

    // ---- 串联所有阶段 ----
    auto* sequence = new QSequentialAnimationGroup;
    sequence->addAnimation(fade_in);
    sequence->addAnimation(move_up);
    sequence->addAnimation(phase3);

    // 动画结束后清理
    connect(sequence, &QSequentialAnimationGroup::finished, this, [this]() {
        splash_label_->hide();
        // 不移除透明度效果，避免重绘闪烁
        // 效果已为1.0，性能开销可忽略
    });

    sequence->start(QAbstractAnimation::DeleteWhenStopped);
}

} // namespace keyfreeze
