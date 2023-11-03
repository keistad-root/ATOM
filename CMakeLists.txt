cmake_minimum_required(VERSION 3.2)
project(myProject)

# by default build optimized code with debug symbols
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE RELWITHDEBINFO)
  # set(CMAKE_BUILD_TYPE ASAN)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# require C++11-capable compiler
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.9")
    message(WARNING "Compiler with incomplete C++11 support detected: functionality limited")
    set(OLD_CXX true)
  endif()
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  message("clang, undefined dynamic_lookup")
  set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -undefined dynamic_lookup")
endif()

find_package(ROOT)
find_package(ROOT REQUIRED COMPONENTS Core Tree TreePlayer Gpad Hist Physics)
find_package(ROOT REQUIRED COMPONENTS RIO Net Hist Graf Graf3d Gpad Tree Rint Postscript Matrix Physics MathCore Thread)

# set C++ standard ensuring compatibility with ROOT
set(CMAKE_CXX_STANDARD 17)
if(ROOT_cxx17_FOUND)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
elseif(ROOT_cxx14_FOUND)
  set(CMAKE_CXX_STANDARD 14)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
elseif(ROOT_cxx11_FOUND)
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()
message(STATUS "Using C++${CMAKE_CXX_STANDARD}")

add_compile_options("-Wall")
add_compile_options("-Wpedantic")
add_compile_options("-Wno-unused-variable")
add_compile_options("-Wunused-variable")
add_compile_options("-Wno-uninitialized")

add_compile_options("-O1")
add_compile_options("-g")

# check for build in the source tree
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(STATUS "Building in source tree")
  set(BUILDINSOURCE true)
endif()

message(STATUS "In Project Directory: ${PROJECT_SOURCE_DIR}")
message(STATUS "ROOT Include Directory: ${ROOT_INCLUDE_DIRS}")

file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)

file(COPY ${PROJECT_SOURCE_DIR}/include DESTINATION ${CMAKE_BINARY_DIR} )

message(STATUS "found inc ${headers}")
message(STATUS "found src ${sources}")

add_library(myProject SHARED ${sources})

# target_link_libraries(myProject PUBLIC ROOT::Core ROOT::Gpad ROOT::Hist ROOT::TreePlayer ROOT::Tree)

include_directories(include)
add_subdirectory(exe)
include_directories(${ROOT_INCLUDE_DIRS})

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/output)