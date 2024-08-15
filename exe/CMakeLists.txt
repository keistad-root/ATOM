# Set the exe file set
set(TEST_EXE
    ExperimentAnalysis
    # simulation
    ThresholdAnalysis
    # SimulationAnalysis
    # GarfieldSimulation
    Merge
    CompareExperimentData
    entrySimulation
)

# for now put the executables in the build root for compatibility with existing scripts
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
# Line exe files to their include files
foreach(exe ${TEST_EXE})
    add_executable(${exe} ${exe}.cpp)
    foreach(HEADER_PATH ${HEADERS})
        get_filename_component(DIR ${HEADER_PATH} DIRECTORY)
        target_include_directories(${exe} PRIVATE ${CMAKE_SOURCE_DIR}/${DIR})
    endforeach()
    target_link_libraries(${exe} myProject ROOT::Core ROOT::Gpad ROOT::Hist ROOT::Tree)
    # target_link_libraries(${exe} ${Geant4_LIBRARIES} myProject ROOT::Core ROOT::Gpad ROOT::Hist ROOT::Tree Garfield::Garfield)
endforeach()
