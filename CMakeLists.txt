cmake_minimum_required(VERSION 3.5)

project(QBlueprint VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(OpenCV QUIET)
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        qblueprint.h qblueprint.cpp qblueprint.ui
        qblueprintnode.h qblueprintnode.cpp
        qblueprintport.h qblueprintport.cpp
        qblueprintconnection.h qblueprintconnection.cpp
        qnodefactory.h qnodefactory.cpp
        testclass.h testclass.cpp
#        qinputnode.h qinputnode.cpp
#        qoutputnode.h qoutputnode.cpp
        alluse.h alluse.cpp
        enterlimiter.h enterlimiter.cpp
        math.h math.cpp
        imagelabel.h imagelabel.cpp
        opencv.h opencv.cpp
        qts.h qts.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QBlueprint
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        enterlimiter.h enterlimiter.cpp
        imagelabel.h imagelabel.cpp
        qts.h qts.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET QBlueprint APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(QBlueprint SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(QBlueprint
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(QBlueprint PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
if(OpenCV_FOUND)
    target_link_libraries(QBlueprint PRIVATE ${OpenCV_LIBS})
    target_compile_definitions(QBlueprint PRIVATE OPENCV_FOUND)
endif()
set_target_properties(QBlueprint PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS QBlueprint
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QBlueprint)
endif()
