project(Sniffer)

cmake_minimum_required(VERSION 2.8)

include_directories(Ethernet/)

aux_source_directory(Ethernet/ SRC_ETHERNET)
aux_source_directory(. SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST} ${SRC_ETHERNET})
