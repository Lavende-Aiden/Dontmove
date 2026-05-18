#pragma once

class QWidget;

namespace keyfreeze {

// Windows 11 风格工具：应用 Fluent Design 风格的 QSS 样式表
class Win11Style {
public:
    // 对整个 QApplication 应用 Win11 样式表
    static void applyStyleSheet();

    // 尝试启用 Windows 11 Mica 云母背景效果（Win11 22H2+ 可用，旧系统自动跳过）
    static void enableMica(QWidget* window);
};

} // namespace keyfreeze
