cmake_minimum_required(VERSION 3.22)

project(task CXX C)

set(CMAKE_CXX_STANDARD 20)


include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)
# FetchContent_GetProperties(nlohmann)




FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)


include_directories(task)

add_subdirectory(task)

enable_testing()
add_subdirectory(test)