cmake_minimum_required(VERSION 3.16)

project(cryptominder VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        src/main.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(cryptominder
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
            headers/cryptominder.h src/cryptominder.cpp src/ui/cryptominder.ui
            headers/receive.h src/receive.cpp src/ui/receive.ui
            headers/send.h src/send.cpp src/ui/send.ui
            headers/addwallet.h src/addwallet.cpp src/ui/addwallet.ui
            headers/deletewallet.h src/deletewallet.cpp src/ui/deletewallet.ui
        headers/Account.h src/Account.cpp
        headers/Wallet.h src/Wallet.cpp
        headers/DataBaseSchema.h src/DataBaseSchema.cpp
        headers/Transaction.h src/Transaction.cpp
        headers/IPersistable.h
        headers/ITransferable.h
            headers/basedb.h
        headers/UpdVector.h
        headers/Iterator.h
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET cryptominder APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(cryptominder SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(cryptominder
            ${PROJECT_SOURCES}
        )
    endif()
endif()

include_directories("/opt/homebrew/Cellar/libpqxx/7.9.2/include")
target_link_libraries(cryptominder PRIVATE /opt/homebrew/Cellar/libpqxx/7.9.2/lib/libpqxx-7.9.dylib)
target_link_libraries(cryptominder PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(cryptominder PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/headers")
target_include_directories(cryptominder PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(cryptominder PRIVATE "src/ui")
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.cryptominder)
endif()
set_target_properties(cryptominder PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS cryptominder
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(cryptominder)
endif()
