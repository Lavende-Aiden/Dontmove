#include "win11_style.h"

#include <QApplication>
#include <QWidget>

#ifdef Q_OS_WIN
#include <Windows.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#endif

namespace keyfreeze {

static const char* kWin11QSS = R"QSS(
/* ================================================================
   Windows 11 Fluent Design QSS — Dontmove
   ================================================================ */

/* ===== 全局 ===== */
* {
    font-family: "Segoe UI Variable", "Segoe UI", sans-serif;
    font-size: 14px;
}

QMainWindow, QWidget#centralWidget {
    background-color: #ffffff;
}

QWidget {
    background-color: transparent;
}

/* 设置窗口白底 */
QWidget#settingsWindowRoot {
    background-color: #ffffff;
}

/* ===== 自定义标题栏 ===== */
QWidget#titleBar {
    background-color: #ffffff;
    border-bottom: 1px solid #ebebeb;
    min-height: 36px;
    max-height: 36px;
}

QLabel#titleBarLabel {
    font-size: 12px;
    font-weight: 400;
    color: #1a1a1a;
    background-color: transparent;
    padding: 0px 4px;
}

QLabel#titleBarIcon {
    background-color: transparent;
    padding: 0px 4px 0px 0px;
}

/* ===== 启动闪屏标签 ===== */
QLabel#splashLabel {
    font-size: 28px;
    font-weight: 700;
    color: #1a1a1a;
    background-color: transparent;
}

QPushButton#topButton, QPushButton#minimizeButton, QPushButton#closeButton {
    background-color: transparent;
    border: none;
    border-radius: 0px;
    padding: 0px;
    min-width: 46px;
    max-width: 46px;
    min-height: 36px;
    max-height: 36px;
}

QPushButton#topButton:hover {
    background-color: #e9e9e9;
}

QPushButton#topButton[pinned="true"] {
    background-color: #e5f1fb;
}

QPushButton#topButton[pinned="true"]:hover {
    background-color: #cce4f7;
}

QPushButton#minimizeButton:hover {
    background-color: #e9e9e9;
}

QPushButton#closeButton:hover {
    background-color: #c42b1c;
}

QPushButton#topButton:focus, QPushButton#minimizeButton:focus, QPushButton#closeButton:focus {
    outline: none;
    border: none;
}

/* 内容区域白底 */
QWidget#contentWidget {
    background-color: #ffffff;
}

/* ===== 标签 ===== */
QLabel {
    color: #1a1a1a;
    background-color: transparent;
}

QLabel#titleLabel {
    font-size: 28px;
    font-weight: 700;
    color: #1a1a1a;
    padding: 0px 0px 4px 0px;
}

QLabel#statusLabel {
    font-size: 15px;
    font-weight: 400;
    color: #616161;
    padding: 2px 0px;
}

QLabel#timerLabel {
    font-size: 36px;
    font-weight: 700;
    color: #0078d4;
    padding: 12px 0px;
}

QLabel#hintLabel {
    font-size: 12px;
    color: #a0a0a0;
    padding: 4px 0px 0px 0px;
}

QLabel#lockedStatusLabel {
    font-size: 15px;
    font-weight: 400;
    color: #c42b1c;
    padding: 2px 0px;
}

/* ===== 按钮（通用/次要） ===== */
QPushButton {
    background-color: #ffffff;
    border: 1px solid #d6d6d6;
    border-radius: 6px;
    padding: 8px 20px;
    color: #1a1a1a;
    min-height: 32px;
}

QPushButton:hover {
    background-color: #f5f5f5;
    border-color: #c4c4c4;
}

QPushButton:pressed {
    background-color: #e8e8e8;
    border-color: #b3b3b3;
}

QPushButton:disabled {
    background-color: #ffffff;
    color: #a0a0a0;
    border-color: #e0e0e0;
}

QPushButton:focus {
    border-color: #0078d4;
    outline: none;
}

/* ===== 主按钮（强调色/切换） ===== */
QPushButton#toggleButton {
    background-color: #0078d4;
    border: none;
    border-radius: 8px;
    padding: 16px 48px;
    color: #ffffff;
    font-size: 16px;
    font-weight: 600;
    min-height: 48px;
    min-width: 220px;
}

QPushButton#toggleButton:hover {
    background-color: #106ebe;
}

QPushButton#toggleButton:pressed {
    background-color: #005a9e;
}

QPushButton#toggleButton:focus {
    outline: none;
    border: none;
}

/* 已锁定状态：红色按钮 */
QPushButton#toggleButton[locked="true"] {
    background-color: #c42b1c;
}

QPushButton#toggleButton[locked="true"]:hover {
    background-color: #a4261a;
}

QPushButton#toggleButton[locked="true"]:pressed {
    background-color: #8b1a10;
}

/* 设置按钮（图标，右下角） */
QPushButton#settingsButton {
    background-color: transparent;
    border: none;
    border-radius: 8px;
    padding: 4px;
    color: #8a8a8a;
    min-height: 36px;
    min-width: 36px;
    max-width: 36px;
    max-height: 36px;
}

QPushButton#settingsButton:hover {
    background-color: #f0f0f0;
}

QPushButton#settingsButton:pressed {
    background-color: #e4e4e4;
}

/* ===== 复选框 ===== */
QCheckBox {
    spacing: 10px;
    color: #1a1a1a;
    background-color: transparent;
    padding: 4px 0px;
}

QCheckBox::indicator {
    width: 20px;
    height: 20px;
    border-radius: 5px;
    border: 2px solid #8a8a8a;
    background-color: transparent;
}

QCheckBox::indicator:hover {
    border-color: #606060;
}

QCheckBox::indicator:checked {
    background-color: #0078d4;
    border-color: #0078d4;
}

QCheckBox::indicator:disabled {
    background-color: #ffffff;
    border-color: #c4c4c4;
}

/* ===== 分组框 ===== */
QGroupBox {
    border: 1px solid #ebebeb;
    border-radius: 8px;
    margin-top: 24px;
    padding: 28px 20px 20px 20px;
    font-weight: 600;
    font-size: 14px;
    color: #1a1a1a;
    background-color: #ffffff;
}

QGroupBox::title {
    subcontrol-origin: margin;
    subcontrol-position: top left;
    padding: 2px 10px;
    background-color: #ffffff;
    border-radius: 4px;
}

/* ===== 数值选择框 ===== */
QSpinBox {
    background-color: #ffffff;
    border: 1px solid #d6d6d6;
    border-radius: 6px;
    padding: 6px 10px;
    min-height: 32px;
    color: #202020;
    selection-background-color: #0078d4;
    selection-color: #ffffff;
}

QSpinBox:hover {
    border-color: #b3b3b3;
}

QSpinBox:focus {
    border-color: #0078d4;
}

QSpinBox::up-button {
    subcontrol-origin: border;
    subcontrol-position: top right;
    width: 20px;
    border-left: 1px solid #d6d6d6;
    border-radius: 0 6px 0 0;
    background-color: #fafafa;
}

QSpinBox::up-button:hover {
    background-color: #e9e9e9;
}

QSpinBox::down-button {
    subcontrol-origin: border;
    subcontrol-position: bottom right;
    width: 20px;
    border-left: 1px solid #d6d6d6;
    border-top: 1px solid #d6d6d6;
    border-radius: 0 0 6px 0;
    background-color: #fafafa;
}

QSpinBox::down-button:hover {
    background-color: #e9e9e9;
}

QSpinBox::up-arrow {
    width: 8px;
    height: 8px;
}

QSpinBox::down-arrow {
    width: 8px;
    height: 8px;
}

/* ===== 右键菜单 ===== */
QMenu {
    background-color: #fcfcfc;
    border: 1px solid #e5e5e5;
    border-radius: 8px;
    padding: 4px 0px;
}

QMenu::item {
    padding: 8px 32px 8px 16px;
    border-radius: 4px;
    margin: 2px 4px;
    color: #202020;
}

QMenu::item:selected {
    background-color: #e9e9e9;
}

QMenu::separator {
    height: 1px;
    background-color: #e5e5e5;
    margin: 4px 12px;
}

/* ===== 滚动条 ===== */
QScrollBar:vertical {
    background: transparent;
    width: 8px;
    margin: 0;
}

QScrollBar::handle:vertical {
    background: #c4c4c4;
    border-radius: 4px;
    min-height: 30px;
}

QScrollBar::handle:vertical:hover {
    background: #a0a0a0;
}

QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0;
}

QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
    background: none;
}

/* ===== 工具提示 ===== */
QToolTip {
    background-color: #f5f5f5;
    border: 1px solid #e0e0e0;
    border-radius: 6px;
    padding: 6px 12px;
    color: #202020;
}

/* ===== 分隔线 ===== */
QFrame#separator {
    background-color: #ebebeb;
    max-height: 1px;
    border: none;
    margin: 12px 20px;
}
)QSS";


void Win11Style::applyStyleSheet() {
    qApp->setStyleSheet(kWin11QSS);
}

void Win11Style::enableMica(QWidget* window) {
#ifdef Q_OS_WIN
    if (!window) return;

    HWND hwnd = reinterpret_cast<HWND>(window->winId());
    if (!hwnd) return;

    // DWMWA_SYSTEMBACKDROP_TYPE = 38 (Windows 11 Build 22523+)
    //   0 = Auto, 1 = None, 2 = Mica, 3 = Acrylic, 4 = Tabbed
    int backdropType = 2; // Mica
    HRESULT hr = DwmSetWindowAttribute(hwnd, 38, &backdropType, sizeof(backdropType));
    if (SUCCEEDED(hr)) {
        window->setAttribute(Qt::WA_TranslucentBackground);
    }
#else
    Q_UNUSED(window)
#endif
}

} // namespace keyfreeze
