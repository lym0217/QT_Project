cmake_minimum_required(VERSION 3.19)
project(project LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(project
    WIN32 MACOSX_BUNDLE
    main.cpp



    loginpage.h loginpage.cpp loginpage.ui
    mainWindow.h mainWindow.cpp
    mainpage.h mainpage.cpp mainpage.ui
    resources.qrc
    account.h
    account_manager.h account_manager.cpp

)

target_link_libraries(project
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS project
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET project
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
