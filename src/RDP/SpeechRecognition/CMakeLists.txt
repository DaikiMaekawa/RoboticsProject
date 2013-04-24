cmake_minimum_required(VERSION 2.8)
project(SpeechRecognition)

add_definitions(-DSHARE_DIR="${CMAKE_INSTALL_PREFIX}/${CATKIN_PACKAGE_SHARE_DESTINATION}")

set(Boost_FIND_REQUIRED TRUE)
set(Boost_FIND_QUIETLY TRUE)
set(Boost_USE_MULTITHREADED TRUE)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_DEBUG TRUE)

set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})

find_package(Boost COMPONENTS system thread filesystem regex)
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs message_generation)

set(Include_DIR 
    ${PROJECT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
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

set(SpeechRecognition_SRCS
    main.cpp
    Timer.h
    Timer.cpp
    SpeechRecognitionServer.h
    SpeechRecognitionServer.cpp
    SpeechRecognitionClient.h
    SpeechRecognitionClient.cpp
)

#add_library(SpeechRecognition
#    ${UserRecognition_SRCS}
#)

link_directories(
    ${Boost_LIBRARY_DIRS}
    ${Boost_LIBRARY_DIR}
)

add_executable(SpeechRecognition
    ${SpeechRecognition_SRCS}
)

target_link_libraries(SpeechRecognition
    ${Boost_LIBRARIES}
    ${catkin_LIBRARIES}
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

install(FILES
    julius.jconf
    DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

install(DIRECTORY
    model
    gram
    DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

