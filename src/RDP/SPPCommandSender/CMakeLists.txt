cmake_minimum_required(VERSION 2.8)

set(Boost_FIND_REQUIRED TRUE)
set(Boost_FIND_QUIETLY TRUE)
set(Boost_USE_MULTITHREADED TRUE)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_DEBUG TRUE)

set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})
#set(Boost_USE_STATIC_LIBS TRUE)

find_package(Boost COMPONENTS system thread date_time serialization)
find_package(PythonLibs 2.7)
find_package(SWIG REQUIRED)

include (${SWIG_USE_FILE})

include_directories(
   ${CMAKE_CURRENT_BINARY_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${BOOST_INCLUDE_DIRS}
   ${Boost_INCLUDE_DIRS}
   ${Boost_INCLUDE_DIR}
   ${PYTHON_INCLUDE_PATH}
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
)

SET(SPP_LINKS
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARIES}
    ${Boost_SYSTEM_LIBRARY}
)

SET(INTERFACE_FILES
    synthesis.i
)

SET_SOURCE_FILES_PROPERTIES(${INTERFACE_FILES} PROPERTIES CPLUSPLUS ON)

SET_SOURCE_FILES_PROPERTIES(${INTERFACE_FILES} PROPERTIES SWIG_FLAGS "-threads")

SWIG_ADD_MODULE(synthesis python ${INTERFACE_FILES}
    ${SPP_SRCS}   
)

SWIG_LINK_LIBRARIES(synthesis
    ${SPP_LINKS}
)

