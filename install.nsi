; =============================================================================
; Dontmove 安装脚本 (NSIS)
; 使用方法: makensis install.nsi
; 需求: NSIS 3.x + Unicode 版本
; =============================================================================

; ---------- 基础配置 ----------
!define PRODUCT_NAME      "Dontmove"
!define PRODUCT_VERSION   "0.1.0"
!define PRODUCT_PUBLISHER "Aiden Team"
!define PRODUCT_EXE       "Dontmove.exe"
!define PRODUCT_WEBINFO   "https://github.com/pierce/Dontmove"

; 从构建输出目录读取文件
!define BUILD_DIR "${__FILEDIR__}\build\Release"

; ---------- Unicode 支持 ----------
Unicode true

; ---------- 安装包属性 ----------
Name            "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile         "${PRODUCT_NAME}-${PRODUCT_VERSION}-setup.exe"
InstallDir      "$PROGRAMFILES64\${PRODUCT_NAME}"
InstallDirRegKey HKLM "Software\${PRODUCT_NAME}" "InstallDir"
RequestExecutionLevel admin

; ---------- 现代界面 (MUI2) ----------
!include "MUI2.nsh"
!include "FileFunc.nsh"
!include "WordFunc.nsh"
!include "nsDialogs.nsh"
!include "LogicLib.nsh"

; --- 界面设置 ---
!define MUI_ABORTWARNING
!define MUI_ICON               "${__FILEDIR__}\resources\icons\app.ico"
!define MUI_UNICON             "${__FILEDIR__}\resources\icons\app.ico"
; 可选: 自定义欢迎页位图 (150x57)
; !define MUI_WELCOMEFINISHPAGE_BITMAP "banner.bmp"
; 可选: 自定义头部位图 (150x35)
; !define MUI_HEADERIMAGE
; !define MUI_HEADERIMAGE_BITMAP       "header.bmp"

; --- 页面顺序 ---
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "$(LICENSE_TEXT)"
!insertmacro MUI_PAGE_DIRECTORY
Page custom CreateShortcutsPage LeaveShortcutsPage
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

; --- 卸载页面 ---
!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

; --- 语言 ---
!insertmacro MUI_LANGUAGE "SimpChinese"
!insertmacro MUI_LANGUAGE "English"

; --- 多语言许可协议 ---
LicenseLangString LICENSE_TEXT ${LANG_SimpChinese} "license_cn.txt"
LicenseLangString LICENSE_TEXT ${LANG_ENGLISH}     "license_en.txt"

; --- 多语言字符串 ---
LangString PRODUCT_NAME_STR    ${LANG_SimpChinese} "${PRODUCT_NAME}"
LangString PRODUCT_NAME_STR    ${LANG_ENGLISH}     "${PRODUCT_NAME}"
LangString DESKTOP_SHORTCUT    ${LANG_SimpChinese} "创建桌面快捷方式"
LangString DESKTOP_SHORTCUT    ${LANG_ENGLISH}     "Create desktop shortcut"
LangString STARTMENU_SHORTCUT  ${LANG_SimpChinese} "创建开始菜单快捷方式"
LangString STARTMENU_SHORTCUT  ${LANG_ENGLISH}     "Create Start Menu shortcut"
LangString AUTOSTART_SHORTCUT  ${LANG_SimpChinese} "开机自动启动"
LangString AUTOSTART_SHORTCUT  ${LANG_ENGLISH}     "Launch on startup"
LangString UNINSTALL_CONFIRM   ${LANG_SimpChinese} "确定要完全移除 ${PRODUCT_NAME} 及其所有组件吗？"
LangString UNINSTALL_CONFIRM   ${LANG_ENGLISH}     "Are you sure you want to completely remove ${PRODUCT_NAME} and all of its components?"
LangString UNINSTALL_DONE      ${LANG_SimpChinese} "${PRODUCT_NAME} 已成功从您的计算机移除。"
LangString UNINSTALL_DONE      ${LANG_ENGLISH}     "${PRODUCT_NAME} has been successfully removed from your computer."
LangString SHORTCUTS_TITLE     ${LANG_SimpChinese} "快捷方式选项"
LangString SHORTCUTS_TITLE     ${LANG_ENGLISH}     "Shortcut Options"
LangString SHORTCUTS_SUBTITLE  ${LANG_SimpChinese} "选择要创建的快捷方式"
LangString SHORTCUTS_SUBTITLE  ${LANG_ENGLISH}     "Choose which shortcuts to create"

; ---------- 快捷方式选项变量 ----------
Var CreateDesktopShortcut
Var CreateStartMenuShortcut
Var CreateAutoStart

; =============================================================================
; 快捷方式自定义页面
; =============================================================================
Function CreateShortcutsPage
    !insertmacro MUI_HEADER_TEXT "$(SHORTCUTS_TITLE)" "$(SHORTCUTS_SUBTITLE)"

    nsDialogs::Create 1018
    Pop $0

    ${If} $0 == "error"
        Abort
    ${EndIf}

    ; 距离顶部一些间距
    ${NSD_CreateCheckbox} 10u 20u 280u 12u "$(DESKTOP_SHORTCUT)"
    Pop $0
    ${NSD_SetState} $0 ${BST_CHECKED}  ; 默认勾选
    StrCpy $CreateDesktopShortcut $0

    ${NSD_CreateCheckbox} 10u 40u 280u 12u "$(STARTMENU_SHORTCUT)"
    Pop $0
    ${NSD_SetState} $0 ${BST_CHECKED}  ; 默认勾选
    StrCpy $CreateStartMenuShortcut $0

    ${NSD_CreateCheckbox} 10u 60u 280u 12u "$(AUTOSTART_SHORTCUT)"
    Pop $0
    ${NSD_SetState} $0 ${BST_UNCHECKED}  ; 默认不勾选
    StrCpy $CreateAutoStart $0

    nsDialogs::Show
FunctionEnd

Function LeaveShortcutsPage
    ; 读取复选框状态
    ${NSD_GetState} $CreateDesktopShortcut $0
    StrCpy $CreateDesktopShortcut $0

    ${NSD_GetState} $CreateStartMenuShortcut $0
    StrCpy $CreateStartMenuShortcut $0

    ${NSD_GetState} $CreateAutoStart $0
    StrCpy $CreateAutoStart $0
FunctionEnd

; =============================================================================
; 安装区段
; =============================================================================
Section "!${PRODUCT_NAME}" SEC_MAIN
    SetOutPath "$INSTDIR"
    SetOverwrite on

    ; --- 主程序 ---
    File "${BUILD_DIR}\${PRODUCT_EXE}"

    ; --- Qt 运行时 DLL ---
    File "${BUILD_DIR}\Qt6Core.dll"
    File "${BUILD_DIR}\Qt6Gui.dll"
    File "${BUILD_DIR}\Qt6Widgets.dll"
    File "${BUILD_DIR}\Qt6Svg.dll"
    File "${BUILD_DIR}\Qt6Network.dll"

    ; --- 渲染依赖 ---
    File "${BUILD_DIR}\d3dcompiler_47.dll"
    File "${BUILD_DIR}\dxcompiler.dll"
    File "${BUILD_DIR}\dxil.dll"
    File "${BUILD_DIR}\opengl32sw.dll"

    ; --- Qt 插件子目录 ---
    SetOutPath "$INSTDIR\platforms"
    File "${BUILD_DIR}\platforms\qwindows.dll"

    SetOutPath "$INSTDIR\styles"
    File "${BUILD_DIR}\styles\qmodernwindowsstyle.dll"

    SetOutPath "$INSTDIR\imageformats"
    File "${BUILD_DIR}\imageformats\qico.dll"
    File "${BUILD_DIR}\imageformats\qjpeg.dll"
    File "${BUILD_DIR}\imageformats\qgif.dll"
    File "${BUILD_DIR}\imageformats\qsvg.dll"

    SetOutPath "$INSTDIR\iconengines"
    File "${BUILD_DIR}\iconengines\qsvgicon.dll"

    SetOutPath "$INSTDIR\generic"
    File "${BUILD_DIR}\generic\qtuiotouchplugin.dll"

    SetOutPath "$INSTDIR\networkinformation"
    File "${BUILD_DIR}\networkinformation\qnetworklistmanager.dll"

    SetOutPath "$INSTDIR\tls"
    File "${BUILD_DIR}\tls\qcertonlybackend.dll"
    File "${BUILD_DIR}\tls\qschannelbackend.dll"

    ; --- 创建日志目录 ---
    SetOutPath "$INSTDIR"
    CreateDirectory "$INSTDIR\logs"

    ; --- 写入注册表 ---
    WriteRegStr HKLM "Software\${PRODUCT_NAME}" "InstallDir" "$INSTDIR"
    WriteRegStr HKLM "Software\${PRODUCT_NAME}" "Version"   "${PRODUCT_VERSION}"

    ; --- 写入卸载信息 ---
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" \
        "DisplayName"     "${PRODUCT_NAME}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" \
        "DisplayVersion"  "${PRODUCT_VERSION}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" \
        "Publisher"       "${PRODUCT_PUBLISHER}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" \
        "UninstallString" '"$INSTDIR\uninstall.exe"'
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" \
        "InstallLocation" '"$INSTDIR"'
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" \
        "DisplayIcon"     '"$INSTDIR\${PRODUCT_EXE}"'
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" \
        "NoModify"  1
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" \
        "NoRepair"  1

    ; 计算安装大小
    ${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
    IntFmt $0 "0x%08X" $0
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" \
        "EstimatedSize" "$0"

    ; --- 生成卸载程序 ---
    WriteUninstaller "$INSTDIR\uninstall.exe"

    ; --- 快捷方式 ---
    ${If} $CreateDesktopShortcut == ${BST_CHECKED}
        CreateShortCut "$DESKTOP\${PRODUCT_NAME}.lnk" "$INSTDIR\${PRODUCT_EXE}" "" "$INSTDIR\${PRODUCT_EXE}" 0
    ${EndIf}

    ${If} $CreateStartMenuShortcut == ${BST_CHECKED}
        CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
        CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk"     "$INSTDIR\${PRODUCT_EXE}" "" "$INSTDIR\${PRODUCT_EXE}" 0
        CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\卸载.lnk"                "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
    ${EndIf}

    ${If} $CreateAutoStart == ${BST_CHECKED}
        WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "${PRODUCT_NAME}" '"$INSTDIR\${PRODUCT_EXE}"'
    ${EndIf}
SectionEnd

; =============================================================================
; 卸载区段
; =============================================================================
Section "Uninstall"
    ; --- 删除快捷方式 ---
    Delete "$DESKTOP\${PRODUCT_NAME}.lnk"
    RMDir /r "$SMPROGRAMS\${PRODUCT_NAME}"

    ; --- 删除开机启动 ---
    DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "${PRODUCT_NAME}"

    ; --- 删除主程序和 DLL ---
    Delete "$INSTDIR\${PRODUCT_EXE}"
    Delete "$INSTDIR\Qt6Core.dll"
    Delete "$INSTDIR\Qt6Gui.dll"
    Delete "$INSTDIR\Qt6Widgets.dll"
    Delete "$INSTDIR\Qt6Svg.dll"
    Delete "$INSTDIR\Qt6Network.dll"
    Delete "$INSTDIR\d3dcompiler_47.dll"
    Delete "$INSTDIR\dxcompiler.dll"
    Delete "$INSTDIR\dxil.dll"
    Delete "$INSTDIR\opengl32sw.dll"

    ; --- 删除插件目录 ---
    Delete "$INSTDIR\platforms\qwindows.dll"
    RMDir "$INSTDIR\platforms"

    Delete "$INSTDIR\styles\qmodernwindowsstyle.dll"
    RMDir "$INSTDIR\styles"

    Delete "$INSTDIR\imageformats\qico.dll"
    Delete "$INSTDIR\imageformats\qjpeg.dll"
    Delete "$INSTDIR\imageformats\qgif.dll"
    Delete "$INSTDIR\imageformats\qsvg.dll"
    RMDir "$INSTDIR\imageformats"

    Delete "$INSTDIR\iconengines\qsvgicon.dll"
    RMDir "$INSTDIR\iconengines"

    Delete "$INSTDIR\generic\qtuiotouchplugin.dll"
    RMDir "$INSTDIR\generic"

    Delete "$INSTDIR\networkinformation\qnetworklistmanager.dll"
    RMDir "$INSTDIR\networkinformation"

    Delete "$INSTDIR\tls\qcertonlybackend.dll"
    Delete "$INSTDIR\tls\qschannelbackend.dll"
    RMDir "$INSTDIR\tls"

    ; --- 删除日志和配置（用户数据） ---
    RMDir /r "$INSTDIR\logs"
    Delete "$INSTDIR\config.ini"

    ; --- 删除卸载程序自身 ---
    Delete "$INSTDIR\uninstall.exe"

    ; --- 移除安装目录（如果为空） ---
    RMDir "$INSTDIR"

    ; --- 清理注册表 ---
    DeleteRegKey HKLM "Software\${PRODUCT_NAME}"
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
SectionEnd
