set(atom_build_optons)


# ATOM_BUILD_OPTION name defvalue [description]
function(ATOM_BUILD_OPTION opt defvalue)
	if(ARGN)
		set(description ${ARGN})
	else()
		set(description " ")
	endif()
	set(${opt}_defvalue	${defvalue} PARENT_SCOPE)
	set(${opt}_description ${description} PARENT_SCOPE)
	set(atom_build_options ${atom_build_options} ${opt} PARENT_SCOPE)
endfunction()

# ATOM_APPLY_OPTIONS()
function(ATOM_APPLY_OPTIONS)
	foreach(opt ${atom_build_options})
		option(${opt} "${${opt}_description}" ${${opt}_defvalue})
	endforeach()
endfunction()

# ATOM_GET_OPTIONS(result ENABLED)
function(ATOM_GET_OPTIONS result)
	CMAKE_PARSE_ARGUMENTS(ARG "ENABLED" "" "" ${ARGN})
	set(enabled)
	foreach(opt ${atom_build_options})
		if(ARG_ENABLED)
			if(${opt})
				set(enabled "${enabled} ${opt}")
			endif()
		else()
			set(enabled "${enabled} ${opt}")
		endif()
	endforeach()
	set(${result} "${enabled}" PARENT_SCOPE)
endfunction()

#ATOM_SHOW_ENABLED_OPTIONS()
function(ATOM_SHOW_ENABLED_OPTIONS)
	set(enabled_opts)
	ATOM_GET_OPTIONS(enabled_opts ENABLED)
	foreach(opt ${enabled_opts})
		message(STATUS "Enabled support for: ${opt}")
	endforeach()
endfunction()

#ROOT_WRITE_OPTIONS(file )

function(ATOM_WRITE_OPTIONS file)
	file(WRITE ${file} "#---Options enabled for the build of ATOM-----------------------------------------------\n")
	foreach(opt ${root_build_options})
		if(${opt})
		file(APPEND ${file} "set(${opt} ON)\n")
		else()
		file(APPEND ${file} "set(${opt} OFF)\n")
		endif()
	endforeach()
endfunction()

ATOM_BUILD_OPTION(alpide OFF "Analysis tools for ALPIDE")
ATOM_BUILD_OPTION(apts OFF "Analysis tools for APTS")
ATOM_BUILD_OPTION(chip OFF "Mother tools for ALPIDE and APTS")
ATOM_BUILD_OPTION(geant4 OFF "Simulation tools for Geant4")
ATOM_BUILD_OPTION(pycpp OFF "Helpful tools for cpp form python library")

option(pidexp "Analysis tools for experiment data in pid" OFF)
option(pidgeant "Analysis tools for GEANT4 simulation data in pid" OFF)
option(pidgarfield "Analysis tools for GARFIELD simulation data in pid" OFF)
option(alpidedaq "Decoding and analysis raw ALPIDE data from experiment by daq board" OFF)
option(aptsdaq "Decoding and analysis raw APTS data from experiment by daq board" OFF)

if(pidexp) 
	set(alpide ON)
	set(chip ON)
	set(pycpp ON)
endif()

if(pidgeant)
	set(alpide ON)
	set(chip ON)
	set(geant4 ON)
	set(pycpp ON)
endif()

if(pidgarfield)
	set(alpide ON)
	set(chip ON)
	set(geant4 ON)
	set(pycpp ON)
endif()

if(alpidedaq)
	set(alpide ON)
	set(chip ON)
	set(pycpp ON)
endif()

if(aptsdaq)
	set(apts ON)
	set(chip ON)
	set(pycpp ON)
endif()

ATOM_APPLY_OPTIONS()

include_regular_expression("^[^.]+$|[.]h$|[.]icc$|[.]hxx$|[.]hpp$")

