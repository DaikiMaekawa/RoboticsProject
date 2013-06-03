cmake_minimum_required(VERSION 2.8)
project(SPP)

set(Boost_FIND_REQUIRED TRUE)
set(Boost_FIND_QUIETLY TRUE)
set(Boost_USE_MULTITHREADED TRUE)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_DEBUG TRUE)

set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})

find_package(Boost COMPONENTS system thread date_time serialization)
find_package(catkin REQUIRED COMPONENTS roscpp message_generation)

include_directories(
   ${CMAKE_CURRENT_BINARY_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${BOOST_INCLUDE_DIRS}
   ${Boost_INCLUDE_DIR}
   ${PROJECT_SOURCE_DIR}
   ${PROJECT_BINARY_DIR}
)

link_directories(
   ${Boost_LIBRARY_DIRS}
   ${Boost_LIBRARY_DIR}
)

SET(SPP_SRCS
    SerialCommunication.h
    SerialCommunication.cpp
    SPPCommand.h
    SPPBoard.h
    SPPBoard.cpp
    CommandSenderServer.h
    CommandSenderServer.cpp
    CommandSenderClient.h
    CommandSenderClient.cpp
    main.cpp
)

SET(SPP_LINKS
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARIES}
    ${Boost_SYSTEM_LIBRARY}
)

add_executable(SPP
    ${SPP_SRCS}
)

target_link_libraries(SPP
    ${catkin_LIBRARIES}
    ${Boost_LIBRARIES}
    ${Boost_SYSTEM_LIBRARY}
)


