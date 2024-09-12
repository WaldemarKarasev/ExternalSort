set(TARGET external_sort_lib)

get_filename_component(SORT_INCLUDE_PATH "." ABSOLUTE)
get_filename_component(SORT_PATH "." ABSOLUTE)

file(GLOB_RECURSE LIB_CXX_SOURCES ${SORT_PATH}/*.cpp )
file(GLOB_RECURSE LIB_HEADERS ${SORT_PATH}/*.hpp)

message(AAAAAAAAAA ${SORT_INCLUDE_PATH})


add_library(${TARGET} ${LIB_CXX_SOURCES} ${SORT_HEADERS})
target_include_directories(${TARGET} PUBLIC ${SORT_INCLUDE_PATH})

target_link_libraries(${TARGET} nlohmann_json::nlohmann_json)