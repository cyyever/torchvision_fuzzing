cmake_minimum_required(VERSION 3.19)

project(torchvision_fuzzing LANGUAGES CXX)
include(cmake/all.cmake)

find_package(Python3 REQUIRED)
if(Python3_FOUND)
  execute_process(
    COMMAND ${Python3_EXECUTABLE} -c "import torch ; print(torch.__path__[0])"
    RESULT_VARIABLE _exitcode
    OUTPUT_VARIABLE _path)
  if(NOT ${_exitcode} EQUAL 0)
    message(FATAL_ERROR "failed to found pytorch path")
  endif()
  string(STRIP ${_path} pytorch_path)
  list(PREPEND CMAKE_PREFIX_PATH ${pytorch_path}/share/cmake)
endif()

find_package(Torch REQUIRED)
find_package(TorchVision REQUIRED)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/fuzzing.cmake)

find_package(doctest REQUIRED)
# file(GLOB fuzzing_sources ${CMAKE_CURRENT_SOURCE_DIR}/fuzz_test/*.cpp)
file(GLOB fuzzing_sources ${CMAKE_CURRENT_SOURCE_DIR}/fuzz_test/encode_jpeg_test.cpp)

set(ENV{FUZZING_TIMEOUT} 10)
set(ENV{MAX_FUZZING_TIME} 60)
set(ENV{FUZZING_RSS_LIMIT} 8192)
set(ENV{FUZZING_MAX_LEN} 1048576)
foreach(fuzzing_source IN LISTS fuzzing_sources)
  get_filename_component(fuzzing_prog ${fuzzing_source} NAME_WE)
  add_executable(fuzz_${fuzzing_prog} ${fuzzing_source})
  target_link_libraries(fuzz_${fuzzing_prog} PRIVATE ${TORCH_LIBRARIES})
  target_link_libraries(fuzz_${fuzzing_prog} PRIVATE TorchVision::TorchVision)
  add_fuzzing(TARGET fuzz_${fuzzing_prog} )
endforeach()
