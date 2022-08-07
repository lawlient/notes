cmake_minimum_required(VERSION 3.16)

project(xprotobuf)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/build)

find_package(Protobuf REQUIRED)

file(GLOB protos ${CMAKE_CURRENT_SOURCE_DIR}/proto/*.proto)
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${protos})

message("PROTO_SRCS = ${PROTO_SRCS}")
message("PROTO_HDRS = ${PROTO_HDRS}")

add_executable(x main.cpp ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(x ${PROTOBUF_LIBRARIES}) 
