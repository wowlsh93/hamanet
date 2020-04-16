cmake_minimum_required(VERSION 3.16)
project(hamanet)

set(CMAKE_CXX_STANDARD 17)

find_library(PTHREAD_PATH pthread)

add_executable(hamanet lyricspoet.cpp src/net/netio.h src/net/netserver.h src/net/netclient.h src/net/netsession.h src/net/netbase.h src/net/nethandler.h src/util/try.h )
target_link_libraries(hamanet ${PTHREAD_PATH})
