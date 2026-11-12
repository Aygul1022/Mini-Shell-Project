
cmake_minimum_required(VERSION 3.18)

project(LinuxMiniShell LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt 6 and the terminal widget library.
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(PkgConfig REQUIRED)

pkg_check_modules(
    QTermWidget
    REQUIRED
    IMPORTED_TARGET
    qtermwidget6
)

# Existing command-line Mini-Shell.
add_executable(mini-shell
    src/main.cpp
    src/builtins/builtins.cpp
    src/parser/parser.cpp
    src/parser/tokenizer.cpp
    src/process/executor.cpp
    src/process/pipeline_executor.cpp
)

# New graphical terminal application.
add_executable(mini-shell-gui
    src/ui/main.cpp
    src/ui/MainWindow.cpp
    src/ui/MainWindow.h
    src/ui/session/TerminalSession.cpp
    src/ui/session/TerminalSession.h
)

# Generate Qt's meta-object code for signals and slots.
set_target_properties(mini-shell-gui
    PROPERTIES AUTOMOC ON
)

target_link_libraries(mini-shell-gui
    PRIVATE
    Qt6::Widgets
    PkgConfig::QTermWidget
)
