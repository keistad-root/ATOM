cmake_minimum_required(VERSION 3.5)
project(ATOM)

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

option(WITH_GEANT4_UIVIS "Build with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

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
message(STATUS "Geant4 Include Directory: ${GEANT4_INCLUDE_DIRS}")

file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h ${PROJECT_SOURCE_DIR}/include/decoder/*.h ${PROJECT_SOURCE_DIR}/include/event/*.h ${PROJECT_SOURCE_DIR}/include/simulation/*.h ${PROJECT_SOURCE_DIR}/include/clustering/*.h ${PROJECT_SOURCE_DIR}/include/format/*.h ${PROJECT_SOURCE_DIR}/include/threshold/*.h)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/decoder/*.cpp ${PROJECT_SOURCE_DIR}/src/event/*.cpp ${PROJECT_SOURCE_DIR}/src/simulation/*.cpp ${PROJECT_SOURCE_DIR}/src/clustering/*.cpp ${PROJECT_SOURCE_DIR}/src/format/*.cpp ${PROJECT_SOURCE_DIR}/src/threshold/*.cpp)

file(COPY ${PROJECT_SOURCE_DIR}/include DESTINATION ${CMAKE_BINARY_DIR} )

message(STATUS "found inc ${headers}")
message(STATUS "found src ${sources}")

add_library(myProject SHARED ${headers} ${sources})

# target_link_libraries(myProject PUBLIC ROOT::Core ROOT::Gpad ROOT::Hist ROOT::TreePlayer ROOT::Tree)

include_directories(include)
include_directories(include/decoder)
include_directories(include/event)
include_directories(include/simulation)
include_directories(include/clustering)
include_directories(include/format)
include_directories(include/threshold)
add_subdirectory(exe)

include_directories(${Geant4_INCLUDE_DIRS})
include_directories(${ROOT_INCLUDE_DIRS})

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/output)