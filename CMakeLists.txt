# Set cmake minimum required version
cmake_minimum_required(VERSION 3.5)

# Set project name
project(ATOM)

# Set default build type
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE RELWITHDEBINFO)
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

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

include(AtomBuildOptions)

# Find ROOT package
find_package(ROOT)
find_package(ROOT REQUIRED COMPONENTS Core Tree TreePlayer Gpad Hist Physics)
find_package(ROOT REQUIRED COMPONENTS RIO Net Hist Graf Graf3d Gpad Tree Rint Postscript Matrix Physics MathCore Thread)

# Find GEANT4 visualizing driver
option(WITH_GEANT4_UIVIS "Build with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

if(NOT TARGET Garfield::Garfield)
  find_package(Garfield REQUIRED)
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

# Set compiler options
add_compile_options("-Wall")
add_compile_options("-Wpedantic")
add_compile_options("-Wno-unused-variable")
add_compile_options("-Wunused-variable")
add_compile_options("-Wno-uninitialized")

add_compile_options("-O1")
add_compile_options("-g")

# Set for doxygen for documentation
option(BUILD_DOC "Build documentation" ON)

find_package(Doxygen)
if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)
# Check for build in the source directory
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(STATUS "Building in source tree")
  set(BUILDINSOURCE true)
endif()

# Print the path of source directory
message(STATUS "In Project Directory: ${PROJECT_SOURCE_DIR}")
# Print path of include directories of external package
message(STATUS "ROOT Include Directory: ${ROOT_INCLUDE_DIRS}")
message(STATUS "Geant4 Include Directory: ${Geant4_INCLUDE_DIR}")
message(STATUS "Garfield Include Directory: ${Garfield_INCLUDE_DIR}")

# Find header files and source files and store it as variable
file(GLOB_RECURSE HEADERS RELATIVE ${PROJECT_SOURCE_DIR} alpide/*/inc/* apts/inc/* chip/inc/* geant4/*/inc/* pycpp/inc/* pycpp/config/inc/* simulation/inc/*)
file(GLOB_RECURSE SOURCES RELATIVE ${PROJECT_SOURCE_DIR} alpide/*/src/* apts/src/* chip/src/* geant4/*/src/* pycpp/src/* pycpp/config/src/* simulation/src/*)

message(${HEADERS})
# file(GLOB_RECURSE HEADERS "${PROJECT_SOURCE_DIR}/include/*.h")
# file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Add header and source files to library
add_library(myProject SHARED ${SOURCES})

# Find include files and link to source files
foreach(HEADER_PATH ${HEADERS})
    get_filename_component(DIR ${HEADER_PATH} DIRECTORY)
    target_include_directories(myProject PRIVATE ${DIR})
endforeach()

# Add ROOT and Geant4 include files
include_directories(${Geant4_INCLUDE_DIRS})
include_directories(${ROOT_INCLUDE_DIRS})
include_directories(${Garfield_INCLUDE_DIRS})

# Add exe subdirectory for executable cpp files
add_subdirectory(exe)

# Make data directory to deal with input and output data
# file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data)
# file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/output)
# file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/config)