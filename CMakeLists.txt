cmake_minimum_required(VERSION 3.15.0)
# cmake_policy(SET CMP0048 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Do not enable compiler specific extensions, for eg on GCC use -std=c++1z (=c++17) and not -std=gnu++17
set(CMAKE_CXX_EXTENSIONS OFF)

# Use ccache is available, has to be before "project()"
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  # Support Unix Makefiles and Ninja
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

project(OpenStudioBenchmarks VERSION 0.0.1)

# The PROJECT_SOURCE_DIR here is root of this repo, which we will use instead of using CMAKE_SOURCE_DIR
set(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
# We will use BINARY_DIR instead of CMAKE_BINARY_DIR
set(PROJECT_BINARY_DIR "${CMAKE_BINARY_DIR}")

# set openstudio_DIR to the directory that contains the openstudioConfig.cmake
# eg C:\src\OpenStudio\build-vs\_CPack_Packages\win64\ZIP\OpenStudio-3.0.0-rc2+ce27ae6b85-Windows\lib\cmake\openstudio
# Or it'll download a zip/tar.gz for you
include(FindOpenStudioSDK.cmake)

#MESSAGE(FATAL_ERROR "CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}, BINARY_DIR=${BINARY_DIR}, PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}, CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}, CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

#if(POLICY CMP0020)
  #cmake_policy(SET CMP0020 NEW)
#endif()
#if(POLICY CMP0022)
  #cmake_policy(SET CMP0022 NEW)
#endif()
#if(POLICY CMP0042)
  #cmake_policy(SET CMP0042 NEW)
#endif()
#if(POLICY CMP0054)
  #cmake_policy(SET CMP0054 NEW)
#endif()
#if(POLICY CMP0077)
  #cmake_policy(SET CMP0077 NEW)
#endif()

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)

###############################################################################
#                                  N I N J A                                  #
###############################################################################
# Ninja support: has to be atop for it to take effect before anything else is done
# Add Color Output if Using Ninja
macro(AddCXXFlagIfSupported flag test)
  CHECK_CXX_COMPILER_FLAG(${flag} ${test})
  if(${${test}})
    message("adding ${flag}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
  endif()
endmacro()

if("Ninja" STREQUAL ${CMAKE_GENERATOR})
  # Clang
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    AddCXXFlagIfSupported(-fcolor-diagnostics COMPILER_SUPPORTS_fcolor-diagnostics)
  endif()

  # g++
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # For some reason it doesn't say its supported, but it works...
    # AddCXXFlagIfSupported(-fdiagnostics-color COMPILER_SUPPORTS_fdiagnostics-color)
    message(STATUS "Ninja: Forcing -fdiagnostics-color=always")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
  endif()
endif()

# Xcode/Ninja generators undefined MAKE
if(CMAKE_GENERATOR MATCHES "Make")
  set(MAKE "$(MAKE)")
else()
  set(MAKE make)
endif()

###############################################################################
#                         C M A K E    C O N T R O L                          #
###############################################################################

# High level project configuration
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/Products")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/Products")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/Products")

set(LIBRARY_SEARCH_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Release" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Debug")

###############################################################################
#          C O M P I L E R    A N D    S Y S T E M    O P T I O N S           #
###############################################################################

# Compiler and system specific options
if(UNIX)

  #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fPIC -fno-strict-aliasing")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -fno-strict-aliasing")

  # all warnings
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC -fno-strict-aliasing -Winvalid-pch -Wnon-virtual-dtor")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fPIC -fno-strict-aliasing -Winvalid-pch")
  # Treat all warnings as errors, extra errors, and be pedantic
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic-errors -pedantic")
  if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overloaded-virtual -ftemplate-depth=1024")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overloaded-virtual")
    # DLM: had to add this due to issues with boost optional and gcc, may be resolved if we move to std::optional
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-maybe-uninitialized")
  endif()

  # Note: CMAKE_CXX_STANDARD set to 20 should already take care of adding -std=c++20 or equivalent
  if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
  endif()
endif()

#######################################################################
#                              C O N A N                              #
#######################################################################

# Conan

# Note JM 2019-04-24: Another option is to globally set "print_run_commands = True" in ~/.conan/conan.conf
option(CONAN_PRINT_RUN_COMMANDS "Log every conan self.run command" OFF)
mark_as_advanced(CONAN_PRINT_RUN_COMMANDS)
if(CONAN_PRINT_RUN_COMMANDS)
  set(ENV{CONAN_PRINT_RUN_COMMANDS} "1")
  message(STATUS "Setting CONAN_PRINT_RUN_COMMANDS: $ENV{CONAN_PRINT_RUN_COMMANDS}")
else()
  unset(ENV{CONAN_PRINT_RUN_COMMANDS})
  message(STATUS "CONAN_PRINT_RUN_COMMANDS: $ENV{CONAN_PRINT_RUN_COMMANDS}")
endif()

set(BUILD_BENCHMARK TRUE)
# Attempt to run, it'll do the right thing if the calling cmake script already ran it
set(CONAN_INSTALL_FILE "${PROJECT_SOURCE_DIR}/ConanInstall.cmake")
include("${CONAN_INSTALL_FILE}")

# A macro to find a conan related value especially when using multi-config builds (MSVC)
# But it also works with single-config builds
macro(FindValue ValueName)
  set(LocalVar "")
  set(LocalVar $<$<CONFIG:Debug>:${${ValueName}_DEBUG}>$<$<CONFIG:Release>:${${ValueName}_RELEASE}>$<$<CONFIG:RelWithDebInfo>:${${ValueName}_RELWITHDEBINFO}>$<$<CONFIG:MinSizeRel>:${${ValueName}_MINSIZEREL}>
  )
#  list(JOIN LocalVar "" LocalVar)
  string(STRIP ${LocalVar} LocalVar)
  set(CURRENT_${ValueName} $<IF:$<BOOL:${LocalVar}>,${LocalVar},${${ValueName}}>)
  # For debug purposes
  # message(STATUS "Found '${ValueName}' as '${CURRENT_${ValueName}}'")
endmacro()

###############################################################################
#                              E X E C U T A B L E                            #
###############################################################################

# Configure a header file to point to this directory
configure_file("resources/resources.hxx.in" "${PROJECT_BINARY_DIR}/resources.hxx")

configure_file(${PROJECT_SOURCE_DIR}/run_benchmark.py ${PROJECT_BINARY_DIR}/run_benchmark.py COPYONLY)
add_custom_target(run_benchmarks
  COMMAND python run_benchmark.py --quiet --all
  DEPENDS "${PROJECT_BINARY_DIR}/run_benchmark.py"
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

include_directories(${PROJECT_BINARY_DIR})


set(benchmark_src

  model/Vector_remove_vs_copy_Benchmark.cpp
  model/Model_Benchmark.cpp
  model/Model_ModelObjects_Benchmark.cpp
  model/ThermalZoneCombineSpaces_Benchmark.cpp

  utilities/LoadIdfFile_Benchmark.cpp
  utilities/IddObjectParse_Benchmark.cpp
  utilities/IdfObjectParse_Benchmark.cpp
  utilities/Workspace_Benchmark.cpp
  utilities/LoadIdd_Benchmark.cpp
  utilities/IdfFileGetObjects_Benchmark.cpp

  osversion/VersionTranslation_Benchmark.cpp

  energyplus/ForwardTranslator_Benchmark.cpp
)

foreach( bench_file ${benchmark_src} )
  get_filename_component(bench_name ${bench_file} NAME_WE)
  get_filename_component(bench_dir ${bench_file} DIRECTORY)
  message("bench_name=${bench_name}, bench_dir=${bench_dir}")
  add_executable( ${bench_name} ${bench_file} )
  target_link_libraries(${bench_name} PRIVATE
    CONAN_PKG::benchmark
    openstudio::openstudiolib
    project_options
    CONAN_PKG::fmt
  )
  set_target_properties(${bench_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${bench_dir}")
  add_dependencies(run_benchmarks ${bench_name})
endforeach()
