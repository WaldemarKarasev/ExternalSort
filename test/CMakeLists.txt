add_executable(algo algo.cpp ${LIB_CXX_SOURCES} ${SORT_HEADERS})
target_link_libraries(algo
    gtest
    nlohmann_json::nlohmann_json
    external_sort_lib
)

add_executable(device device.cpp ${LIB_CXX_SOURCES} ${SORT_HEADERS})
target_link_libraries(device
    gtest
    nlohmann_json::nlohmann_json
    external_sort_lib
)

add_executable(file file.cpp ${LIB_CXX_SOURCES} ${SORT_HEADERS})
target_link_libraries(file
    gtest
    nlohmann_json::nlohmann_json
    external_sort_lib
)

target_include_directories(algo PUBLIC ${SORT_INCLUDE_PATH})
target_include_directories(device PUBLIC ${SORT_INCLUDE_PATH})
target_include_directories(file PUBLIC ${SORT_INCLUDE_PATH})



gtest_discover_tests(algo)
gtest_discover_tests(device)
gtest_discover_tests(file)