cmake_minimum_required(VERSION 2.8)

project(Sniffer)

SET(NET_ANALIZER_INCLUDE_DIRECTORIES
        "${PROJECT_SOURCE_DIR}/include/Protocols"
        "${PROJECT_SOURCE_DIR}/include/NetworkListener"
        "${PROJECT_SOURCE_DIR}/include/TrafficHandler"
)

include_directories(${NET_ANALIZER_INCLUDE_DIRECTORIES})

set(NET_ANALIZER_SOURCES_LIST
        "${PROJECT_SOURCE_DIR}/src/main.cpp"
        "${PROJECT_SOURCE_DIR}/src/Protocols/IPv4.cpp"
        "${PROJECT_SOURCE_DIR}/src/NetworkListener/NetworkListener.cpp"
        "${PROJECT_SOURCE_DIR}/src/TrafficHandler/EthernetIIHandler.cpp"
        "${PROJECT_SOURCE_DIR}/src/TrafficHandler/Terminator.cpp"
)

add_executable(${PROJECT_NAME} ${NET_ANALIZER_SOURCES_LIST})

