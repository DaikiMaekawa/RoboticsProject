project(MotionCapture)

find_package(Qt4 REQUIRED)
set(QT_USE_QTMAIN true)
set(QT_USE_QTGUI true)
set(QT_USE_QTNETWORK true)
include(${QT_USE_FILE})

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost COMPONENTS system thread REQUIRED)

include_directories(
    ${QT_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}
    ${CMAKE_BINARY_DIR}
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
)

set(MotionCapture_HDRS
    MotionCapture.h
)

set(MotionCapture_UIS
    MotionCapture.ui
)

QT4_ADD_RESOURCES(MotionCapture_RC MotionCapture.qrc)
QT4_WRAP_UI(MotionCapture_UIS ${MotionCapture_UIS})
QT4_WRAP_CPP(MotionCapture_MOC ${MotionCapture_HDRS})

add_executable(MotionCapture WIN32
    ${MotionCapture_SRCS}
    ${MotionCapture_HDRS}
    ${MotionCapture_UIS}
    ${MotionCapture_RC}
    ${MotionCapture_MOC}
)

target_link_libraries(MotionCapture
    ${QT_LIBRARIES}
    ${Boost_LIBRARIES}
)

