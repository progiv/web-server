add_definitions(-Wall -pedantic -O2 -std=c++0x)

set(SOURCES
    web-server.cpp
    server.cpp
    thread.cpp
)

set(LIBS
    #pthread
)

add_executable(web-server ${SOURCES})
target_link_libraries(web-server ${LIBS})
