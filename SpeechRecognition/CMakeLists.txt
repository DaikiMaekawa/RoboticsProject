cmake_minimum_required(VERSION 2.8)
project(SpeechRecognition)

#set(CMAKE_CXX_FLAGS ${CMAKE_CSS_FLAGS} "-fPIC")
set(EXTRA_OPTIONS "-O3 -Wno-deprecated -fPIC -DLINUX")
set_source_files_properties(${SRC_FILES}
    PROPERTIES
    COMPILE_FLAGS "${EXTRA_OPTIONS}"
)

set(Atlas_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

set(Boost_FIND_REQUIRED TRUE)
set(Boost_FIND_QUIETLY TRUE)
set(Boost_USE_MULTITHREADED TRUE)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_DEBUG TRUE)

set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})

FIND_PACKAGE(Boost COMPONENTS system thread filesystem regex)

set(Include_DIR 
    ${PROJECT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Atlas_INCLUDE_DIR}
    ${BOOST_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIR}
    ${PROJECT_BINARY_DIR}
)

if(UNIX)
    set(Include_DIR
        ${Include_DIR}
        /usr/local/lib
        /usr/local/include
        /usr/lib
        /usr/include
    )
else()
    set(Include_DIR
        ${Include_DIR}
        ${Julius_INCLUDE_DIRS}
    )
endif(UNIX)

include_directories(${Include_DIR})

set(Atlas_Source
    ${Atlas_INCLUDE_DIR}/Atlas/Timer.cpp
)

set(UserRecognition_SRCS
    SpeechRecognitionTest.cpp
    SpeechRecognition.h
    SpeechRecognition.cpp
    ${Atlas_Source}
)

add_library(SpeechRecognition
    SHARED ${UserRecognition_SRCS}
)

link_directories(
    ${Boost_LIBRARY_DIRS}
    ${Boost_LIBRARY_DIR}
)

target_link_libraries(SpeechRecognition
    ${Boost_LIBRARIES}
    #${Boost_SYSTEM_LIBRARY}
    julius
    sent
    asound
    m
    pthread
    dl
    z
    pulse
    pulse-simple
    esd
    sndfile
    icuuc
)

add_executable(SpeechRecognitionTest
    SpeechRecognitionTest.cpp
)

target_link_libraries(SpeechRecognitionTest
    SpeechRecognition
)

