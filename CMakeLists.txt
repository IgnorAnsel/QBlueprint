cmake_minimum_required(VERSION 3.5)

project(QBlueprint VERSION 0.1 LANGUAGES CXX)
set(CUDA_TOOLKIT_ROOT_DIR "/usr/local/cuda-11.8")  # 你的 CUDA 安装路径
set(CUDA_INCLUDE_DIRS "${CUDA_TOOLKIT_ROOT_DIR}/include")
set(CUDA_CUDART_LIBRARY "${CUDA_TOOLKIT_ROOT_DIR}/lib64/libcudart.so")
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OpenCV_DIR D:/opencv/opencv/new)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(OpenCV QUIET)
set(PROJECT_SOURCES

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QBlueprint
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        include/enterlimiter.h src/enterlimiter.cpp
        include/imagelabel.h src/imagelabel.cpp
        include/qts.h src/qts.cpp
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
                src/main.cpp
                src/mainwindow.cpp
                include/mainwindow.h
                include/qblueprint.h src/qblueprint.cpp
                include/qblueprintnode.h src/qblueprintnode.cpp
                include/qblueprintport.h src/qblueprintport.cpp
                include/qblueprintconnection.h src/qblueprintconnection.cpp
                include/qnodefactory.h src/qnodefactory.cpp
                include/testclass.h src/testclass.cpp
                #        qinputnode.h qinputnode.cpp
                #        qoutputnode.h qoutputnode.cpp
                include/alluse.h src/alluse.cpp
                include/enterlimiter.h src/enterlimiter.cpp
                include/math.h src/math.cpp
                include/imagelabel.h src/imagelabel.cpp
                include/opencv.h src/opencv.cpp
                include/qts.h src/qts.cpp
                include/ForLoopDebugger.h src/ForLoopDebugger.cpp
                include/ForLoopTester.h src/ForLoopTester.cpp
                include/clickableframe.h
        )
    endif()
endif()

target_link_libraries(QBlueprint PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
include_directories(${OpenCV_INCLUDE_DIRS})

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
