file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers} src/qps/VariableStore.cpp src/qps/VariableStore.h src/qps/QPS.cpp src/qps/QPS.h src/qps/entities/Query.cpp src/qps/entities/Query.h src/qps/parser/QPSParser.cpp src/qps/parser/QPSParser.h)
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


