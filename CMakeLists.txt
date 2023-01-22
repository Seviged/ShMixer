cmake_minimum_required(VERSION 3.5)

add_definitions(-D_WIN32_WINNT=0x0602)

project(ShMixer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

set (${CMAKE_MINGW_IMPLICIT_INCLUDE_DIRECTORIES} "")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)



set(TS_FILES ShMixer_ru_RU.ts)

set(PROJECT_SOURCES
        main.cpp
        shmixer.cpp
        shmixer.h
        shmixer.ui
        windowsAudio/processcatcher.h
        windowsAudio/processcatcher.cpp
        windowsAudio/structs/sessioninfo.h
        windowsAudio/structs/sessioninfo.cpp
        windowsAudio/interfaces/sessionnotificationinterface.h
        windowsAudio/interfaces/sessionnotificationinterface.cpp
        volumecontrol.h
        volumecontrol.cpp
        volumecontrol.ui
        windowsAudio/audiosession.h
        windowsAudio/audiosession.cpp
        windowsAudio/interfaces/audionotificationinterface.h
        windowsAudio/interfaces/audionotificationinterface.cpp
        windowsAudio/device.h
        windowsAudio/device.cpp
        windowsAudio/structs/deviceinfo.h
        windowsAudio/structs/deviceinfo.cpp
        windowsAudio/interfaces/endpointnotificationinterface.h
        windowsAudio/interfaces/endpointnotificationinterface.cpp
        devicevolumecontrol.h
        devicevolumecontrol.cpp
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ShMixer
        ${PROJECT_SOURCES}
    )

#target_include_directories(ShMixer ${WINDOWS_SDK_INCLUDE_DIR})

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(ShMixer SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(ShMixer
            ${PROJECT_SOURCES}
        )
    endif()

    target_include_directories(ShMixer PRIVATE${WINDOWS_SDK_INCLUDE_DIR})

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(ShMixer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets psapi winmm)
