# Set the exe file set
set(TEST_EXE
    manager
    test_new_decoder
    simulation
    ThrAna
    SimulResult
)

# for now put the executables in the build root for compatibility with existing scripts
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Line exe files to their include files
foreach(exe ${TEST_EXE})
    add_executable(${exe} ${exe}.cpp)
    foreach(HEADER_PATH ${HEADERS})
        get_filename_component(DIR ${HEADER_PATH} DIRECTORY)
        target_include_directories(${exe} PRIVATE ${DIR})
    endforeach()
    target_link_libraries(${exe} ${Geant4_LIBRARIES} myProject ROOT::Core ROOT::Gpad ROOT::Hist ROOT::Tree)
endforeach()