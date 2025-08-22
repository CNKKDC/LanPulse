cmake_minimum_required(VERSION 3.12)
project(NetworkDiscovery C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 平台特定配置
if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0600)
    list(APPEND LIBS ws2_32)
else()
    list(APPEND LIBS pthread)
endif()

# 可执行文件
add_executable(broadcaster src/broadcaster.c src/discovery_common.c)
add_executable(responder src/responder.c src/discovery_common.c)

target_link_libraries(broadcaster ${LIBS})
target_link_libraries(responder ${LIBS})