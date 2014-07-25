cmake_minimum_required(VERSION 2.8)

project(Sniffer)

SET(NET_ANALIZER_INCLUDE_DIRECTORIES
        "${PROJECT_SOURCE_DIR}/include/Protocols"
)

include_directories(${NET_ANALIZER_INCLUDE_DIRECTORIES})

set(NET_ANALIZER_SOURCES_LIST
        "${PROJECT_SOURCE_DIR}/src/main.cpp"
        "${PROJECT_SOURCE_DIR}/src/Protocols/EthernetFrame.cpp"
        "${PROJECT_SOURCE_DIR}/src/Protocols/IPv4.cpp"
)

add_executable(${PROJECT_NAME} ${NET_ANALIZER_SOURCES_LIST})

