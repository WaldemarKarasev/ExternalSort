set(TARGET external_sort)


add_executable(${TARGET} ${LIB_CXX_SOURCES} main.cpp)
target_include_directories(${TARGET} PUBLIC ${SORT_INCLUDE_PATH})

target_link_libraries(${TARGET} external_sort_lib nlohmann_json::nlohmann_json)