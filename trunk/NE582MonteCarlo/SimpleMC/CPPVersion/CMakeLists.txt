#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(simpleMC)


#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

#---------------------------------------------------------------------------
# Locate and include header files
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Find Root package
#
if(UNIX)
	if(APPLE)
		set(CMAKE_OSX_SYSROOT "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SKS/MacOSX10.7.sdk")
	endif(APPLE)
	execute_process(COMMAND root-config --incdir OUTPUT_VARIABLE ROOT_INC_DIR)
  include_directories(${ROOT_INC_DIR})
  message(STATUS "Should have set root includes to ${ROOT_INC_DIR}")
   execute_process(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
   execute_process(COMMAND root-config --libs OUTPUT_VARIABLE ROOT_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
   set(CMAKE_EXE_LINKER_FLAGS ${ROOT_LD_FLAGS})
   message(STATUS "Added ROOT Linking Flags: ${ROOT_LD_FLAGS}")
endif(UNIX)
if(WIN32)
  # find_path(ROOT_INC_DIR "C:\Users\LabUser\Documents\Geant4\Tools\root-build-VS")
  find_package(ROOT REQUIRED)
#   include_directories(${ROOT_INC_DIR})
   #include_external_msproject(ROOT.sln "C:\Users\LabUser\Documents\Geant4\Tools\root-build-VS")
endif(WIN32)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Math Library
#
add_executable(simpleMC simpleMC.c ${sources} ${headers})
add_executable(tallyTest tallyTest.c ${sources} ${headers})
if(UNIX)
target_link_libraries(simpleMC)
endif(UNIX)
if(WIN32)
target_link_libraries(simpleMC ${ROOT_LIBRARIES})
endif(WIN32)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS simpleMC DESTINATION bin)
