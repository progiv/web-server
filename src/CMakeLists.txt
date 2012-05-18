add_definitions(-Wall -pedantic -O2 -std=c++0x)

set(SOURCES
    serverThread.cpp
    server.cpp
    thread.cpp
)

set(LIBS
    pthread
)

add_executable(serverThread ${SOURCES})
target_link_libraries(serverThread ${LIBS})
