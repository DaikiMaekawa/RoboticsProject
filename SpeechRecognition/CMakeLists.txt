project(SpeechRecognition)

set(Atlas_Include_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Atlas)

set(Boost_FIND_REQUIRED TRUE)
set(Boost_FIND_QUIETLY TRUE)
set(Boost_USE_MULTITHREADED TRUE)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_DEBUG TRUE)

set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})

FIND_PACKAGE(Boost COMPONENTS system thread filesystem regex)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Atlas_Include_DIR}
    ${BOOST_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIR}
    ${PROJECT_BINARY_DIR}
    /usr/local/lib
    /usr/local/include
)

set(Atlas_Source
    ${Atlas_Include_DIR}/Timer.cpp
)

add_executable(SpeechRecognition 
    SpeechRecognitionTest.cpp
    SpeechRecognition.h
    SpeechRecognition.cpp
    ${Atlas_Source}
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
    icuuc
)
