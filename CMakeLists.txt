cmake_minimum_required(VERSION 3.16)

project(Dontmove VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Help CMake discover Qt6 on Windows when user did not set Qt6_DIR/CMAKE_PREFIX_PATH.
if (WIN32 AND NOT Qt6_DIR)
    set(_qt6_candidate_roots
        "$ENV{QTDIR}"
        "D:/APP/qt/6.9.3/msvc2022_64"
    )

    foreach(_qt_root IN LISTS _qt6_candidate_roots)
        if (_qt_root AND EXISTS "${_qt_root}/lib/cmake/Qt6/Qt6Config.cmake")
            list(PREPEND CMAKE_PREFIX_PATH "${_qt_root}")
            break()
        endif()
    endforeach()
endif()

find_package(Qt6 QUIET COMPONENTS Widgets)
if (NOT Qt6_FOUND)
    message(FATAL_ERROR
        "Qt6 was not found.\n"
        "Please install Qt6 (Widgets) and configure one of these:\n"
        "  1) cmake -S . -B build -DCMAKE_PREFIX_PATH=\"C:/Qt/<version>/msvc2022_64\"\n"
        "  2) cmake -S . -B build -DQt6_DIR=\"C:/Qt/<version>/msvc2022_64/lib/cmake/Qt6\"\n"
        "  3) set QTDIR=C:/Qt/<version>/msvc2022_64"
    )
endif()

set(SOURCES
    src/main.cpp
    src/core/input_blocker.cpp
    src/core/hotkey_manager.cpp
    src/core/emergency_unlocker.cpp
    src/services/tray_service.cpp
    src/services/settings_manager.cpp
    src/services/logger.cpp
    src/platform/win32_api.cpp
    src/ui/main_window.cpp
    src/ui/settings_window.cpp
    src/ui/win11_style.cpp
)

set(HEADERS
    src/core/input_blocker.h
    src/core/hotkey_manager.h
    src/core/emergency_unlocker.h
    src/services/tray_service.h
    src/services/settings_manager.h
    src/services/logger.h
    src/platform/win32_api.h
    src/common/types.h
    src/common/constants.h
    src/ui/main_window.h
    src/ui/settings_window.h
    src/ui/win11_style.h
)

set(RESOURCES
    resources/resources.qrc
)

set(WIN_RESOURCES
    resources/app.rc
)

add_executable(${PROJECT_NAME} WIN32
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
    ${WIN_RESOURCES}
)

# Add source directory to include paths
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Widgets
        dwmapi
)

if (WIN32)
    # Auto-deploy Qt runtime DLLs next to the executable to avoid "app does nothing" on launch.
    get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

    if (WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                    --no-translations
                    --no-compiler-runtime
                    "$<TARGET_FILE:${PROJECT_NAME}>"
            COMMENT "Running windeployqt for ${PROJECT_NAME}"
        )
    else()
        message(WARNING "windeployqt not found. Qt runtime may be missing at launch.")
    endif()
endif()
