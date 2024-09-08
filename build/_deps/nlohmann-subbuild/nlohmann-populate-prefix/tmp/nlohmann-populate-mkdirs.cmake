# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/pingvinus/prog/cpp/test_task/build/_deps/nlohmann-src"
  "/home/pingvinus/prog/cpp/test_task/build/_deps/nlohmann-build"
  "/home/pingvinus/prog/cpp/test_task/build/_deps/nlohmann-subbuild/nlohmann-populate-prefix"
  "/home/pingvinus/prog/cpp/test_task/build/_deps/nlohmann-subbuild/nlohmann-populate-prefix/tmp"
  "/home/pingvinus/prog/cpp/test_task/build/_deps/nlohmann-subbuild/nlohmann-populate-prefix/src/nlohmann-populate-stamp"
  "/home/pingvinus/prog/cpp/test_task/build/_deps/nlohmann-subbuild/nlohmann-populate-prefix/src"
  "/home/pingvinus/prog/cpp/test_task/build/_deps/nlohmann-subbuild/nlohmann-populate-prefix/src/nlohmann-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/pingvinus/prog/cpp/test_task/build/_deps/nlohmann-subbuild/nlohmann-populate-prefix/src/nlohmann-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/pingvinus/prog/cpp/test_task/build/_deps/nlohmann-subbuild/nlohmann-populate-prefix/src/nlohmann-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
