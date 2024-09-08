set(TASK_TARGET task)

get_filename_component(LIB_INCLUDE_PATH ".." ABSOLUTE)
get_filename_component(LIB_PATH "." ABSOLUTE)

file(GLOB_RECURSE LIB_CXX_SOURCES ${LIB_PATH}/*.cpp )
file(GLOB_RECURSE LIB_HEADERS ${LIB_PATH}/*.hpp)

message(${LIB_INCLUDE_PATH})

add_executable(${TASK_TARGET} ${LIB_CXX_SOURCES} ${LIB_HEADERS})
target_include_directories(${TASK_TARGET} PUBLIC ${LIB_INCLUDE_PATH})