cmake_minimum_required(VERSION 2.8)
project(MotionCapture)

find_package(Qt4 REQUIRED)
set(QT_USE_QTMAIN true)
set(QT_USE_QTGUI true)
set(QT_USE_QTNETWORK true)
set(Boost_DEBUG TRUE)
include(${QT_USE_FILE})

#set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

find_package(Boost COMPONENTS system thread REQUIRED)
find_package(catkin REQUIRED COMPONENTS roscpp std_msgs message_generation)

include_directories(
    ${QT_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_BINARY_DIR}
    ${BOOST_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIR}
)

link_directories(
    ${QT_LIBRARY_DIR}
	${Boost_LIBRARY_DIRS}
	${Boost_LIBRARY_DIR}
)

set(MotionCapture_SRCS
    main.cpp
	MotionCapture.cpp
	../UserRecognition/UserRecognitionClient.cpp
)

set(MotionCapture_HDRS
    MotionCapture.h
	../UserRecognition/UserRecognitionClient.h
)

set(MotionCapture_UIS
    MotionCapture.ui
)

QT4_WRAP_UI(MotionCapture_UIS_H ${MotionCapture_UIS})
QT4_WRAP_CPP(MotionCapture_HDRS_MOC ${MotionCapture_HDRS})

add_executable(MotionCapture WIN32
	${MotionCapture_SRCS}
	${MotionCapture_HDRS}
	${MotionCapture_HDRS_MOC}
	${MotionCapture_UIS_H}
)

target_link_libraries(MotionCapture
	${QT_LIBRARIES}
	${Boost_LIBRARIES}
	${catkin_LIBRARIES}
)

