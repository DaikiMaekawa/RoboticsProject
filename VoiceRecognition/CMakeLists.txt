project(test)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    /usr/local/lib
    /usr/local/include
)

add_executable(test 
    #julius-simple.cpp
    main.cpp
    SpeechRecognition.h
    SpeechRecognition.cpp
)

target_link_libraries(test
    libjulius.a
    libsent.a
    asound
    m
    pthread
    dl
    z
    pulse
)
