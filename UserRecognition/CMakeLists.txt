cmake_minimum_required(VERSION 2.8)
project(UserRecognition)

set(Atlas_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

set(Boost_FIND_REQUIRED TRUE)
set(Boost_FIND_QUIETLY TRUE) 
set(Boost_USE_MULTITHREADED TRUE) 
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_DEBUG TRUE)

set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})
#set(Boost_USE_STATIC_LIBS TRUE)

add_definitions(-fPIC)

find_package(Boost COMPONENTS system thread date_time serialization)
find_package(PkgConfig)
find_package(OpenCV)

if(${CMAKE_VERSION} VERSION_LESS 2.8.2)
     pkg_check_modules(PC_OPENNI openni-dev)
else()
     pkg_check_modules(PC_OPENNI QUIET openni-dev)
endif()

set(OPENNI_DEFINITIONS ${PC_OPENNI_CFLAGS_OTHER})

find_path(OPENNI_INCLUDE_DIR XnStatus.h HINTS ${NESTK_ROOT_DIRS_HINTS} ${PC_OPENNI_INCLUDEDIR} ${PC_OPENNI_INCLUDE_DIRS} /usr/include/openni /usr/include/ni
    PATHS "$ENV{PROGRAMFILES}/OpenNI/Include" "$ENV{PROGRAMW6432}/OpenNI/Include" PATH_SUFFIXES openni)

find_library(OPENNI_LIBRARY 
             NAMES OpenNI64 OpenNI
             HINTS ${NESTK_ROOT_DIRS_HINTS} ${PC_OPENNI_LIBDIR} ${PC_OPENNI_LIBRARY_DIRS} /usr/lib
             PATHS "$ENV{PROGRAMFILES}/OpenNI/Lib${OPENNI_SUFFIX}" "$ENV{PROGRAMW6432}/OpenNI/Lib${OPENNI_SUFFIX}"
             PATH_SUFFIXES lib
)


find_library(NITE_LIBRARY
           NAMES XnVNite XnVNITE_1_3_1 XnVNITE_1_4_0 XnVNite_1_4_2 XnVNite_1_5_2
           HINTS ${NESTK_ROOT_DIRS_HINTS} ${PC_OPENNI_LIBDIR} ${PC_OPENNI_LIBRARY_DIRS} /usr/lib
           PATHS "$ENV{PROGRAMFILES}/PrimeSense/NITE/Lib${OPENNI_SUFFIX}" "$ENV{PROGRAMW6432}/PrimeSense/NITE/Lib${OPENNI_SUFFIX}"
           PATH_SUFFIXES lib
)

find_path(NITE_INCLUDE_DIR XnVSessionManager.h
        HINTS ${NESTK_ROOT_DIRS_HINTS} ${PC_OPENNI_INCLUDEDIR} ${PC_OPENNI_INCLUDE_DIRS} /usr/include/openni /usr/include/nite
        PATHS "$ENV{PROGRAMFILES}/PrimeSense/NITE/Include" "$ENV{PROGRAMW6432}/PrimeSense/NITE/Include"
        PATH_SUFFIXES openni)

set(OPENNI_INCLUDE_DIRS ${OPENNI_INCLUDE_DIR} ${NITE_INCLUDE_DIR})
if(APPLE)
    set(OPENNI_LIBRARIES ${OPENNI_LIBRARY} ${NITE_LIBRARY} usb)
else()
    set(OPENNI_LIBRARIES ${OPENNI_LIBRARY} ${NITE_LIBRARY})
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenNI DEFAULT_MSG
OPENNI_LIBRARY OPENNI_INCLUDE_DIR)
    
mark_as_advanced(OPENNI_LIBRARY OPENNI_INCLUDE_DIR)
if(OPENNI_FOUND)
    include_directories(${OPENNI_INCLUDE_DIRS})
    link_directories(${OPENNI_LIBRARY})
    add_definitions(${OPENNI_DEFINITIONS})
    message(STATUS "OpenNI found (include: ${OPENNI_INCLUDE_DIR}, lib: ${OPENNI_LIBRARY})")
endif(OPENNI_FOUND)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${OPENNI_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BOOST_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIR}
    ${PROJECT_BINARY_DIR}
    ${Atlas_INCLUDE_DIR}
)

set(UserRecognition_SRC
    UserDetector.cpp
    UserRecognition.cpp
    Xtion.cpp
    XnErrorChecker.cpp
)

set(UserRecognition_HDR
    UserDetector.h
    UserStatus.h
    UserRecognition.h
    AudioDevice.h
    ImageDevice.h
    Xtion.h
    XnErrorChecker.h
)

add_library(UserRecognition
    ${UserRecognition_SRC}
    ${UserRecognition_HDR}
)

target_link_libraries(UserRecognition 
    ${OPENNI_LIBRARY}
    ${OpenCV_LIBS}
    ${Boost_LIBRARIES}
    ${Boost_SYSTEM_LIBRARY}
)

add_executable(UserRecognitionTest
    UserRecognitionTest.cpp
)

target_link_libraries(UserRecognitionTest
    UserRecognition
)

link_directories(
   ${Boost_LIBRARY_DIRS}
   ${Boost_LIBRARY_DIR}
)

