cmake_minimum_required(VERSION 3.16)
project(MiniShellSessionPrototype LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(PkgConfig REQUIRED)
pkg_check_modules(QTermWidget REQUIRED IMPORTED_TARGET qtermwidget6)
add_executable(minishell-session-test
    main.cpp
    ../../src/ui/session/TerminalSession.cpp
    ../../src/ui/session/TerminalSession.h)
target_include_directories(minishell-session-test PRIVATE ../../src/ui/session)
target_link_libraries(minishell-session-test PRIVATE Qt6::Widgets PkgConfig::QTermWidget)
target_compile_options(minishell-session-test PRIVATE -Wall -Wextra -Wpedantic)
