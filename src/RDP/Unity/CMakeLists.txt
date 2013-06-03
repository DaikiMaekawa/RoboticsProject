cmake_minimum_required(VERSION 2.8)
project(Unity)

set(Boost_FIND_REQUIRED TRUE)
set(Boost_FIND_QUIETLY TRUE)
set(Boost_USE_MULTITHREADED TRUE)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_DEBUG TRUE)

set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})

find_package(Boost COMPONENTS system thread filesystem regex)
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs message_generation)
find_package(PythonLibs 2.7)
find_package(SWIG REQUIRED)

include(${SWIG_USE_FILE})
#include(GenerateExportHeader)

set(Include_DIR 
    ${PROJECT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BOOST_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIR}
    ${PROJECT_BINARY_DIR}
    ${PYTHON_INCLUDE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/../SpeechRecognition
    ${CMAKE_CURRENT_SOURCE_DIR}/../UserRecognition
    ${CMAKE_CURRENT_SOURCE_DIR}/../SPPCommandSender
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

add_definitions(-DSHARE_DIR="${CMAKE_INSTALL_PREFIX}/${CATKIN_PACKAGE_SHARE_DESTINATION}")

set(Unity_SRCS
    Unity.h
    Unity.cpp
    ../SpeechRecognition/SpeechRecognitionClient.h
    ../SpeechRecognition/SpeechRecognitionClient.cpp
    ../UserRecognition/UserRecognitionClient.h
    ../UserRecognition/UserRecognitionClient.cpp
    ../SPPCommandSender/CommandSenderClient.h
    ../SPPCommandSender/CommandSenderClient.cpp
    ../SPPCommandSender/SPPBoard.h
    ../SPPCommandSender/SPPBoard.cpp
)

set(INTERFACE_FILES
    unity.i
)

set_source_files_properties(${INTERFACE_FILES} PROPERTIES CPLUSPLUS ON)
set_source_files_properties(${INTERFACE_FILES} PROPERTIES SWIG_FLAGS "-threads")

SWIG_ADD_MODULE(unity python ${INTERFACE_FILES}
    ${Unity_SRCS}
)

link_directories(
    ${Boost_LIBRARY_DIRS}
    ${Boost_LIBRARY_DIR}
)

add_executable(Unity
    TestUnity.cpp
    ${Unity_SRCS}
)

set(Unity_LINKS
    ${Boost_LIBRARIES}
    ${catkin_LIBRARIES}
)

SWIG_LINK_LIBRARIES(unity
    ${Unity_LINKS}
)

install(TARGETS _unity
    RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../UnityScripts
    LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../UnityScripts
    ARCHIVE DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../UnityScripts
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${outdir}unity.py DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../UnityScripts)

target_link_libraries(Unity
    ${Boost_LIBRARIES}
    ${catkin_LIBRARIES}
    #${Boost_SYSTEM_LIBRARY}
)

