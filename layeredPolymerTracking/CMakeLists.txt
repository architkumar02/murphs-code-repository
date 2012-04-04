#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(layeredPolymerTracking)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all PATHS "/opt/geant4.9.5/lib/Geant4-9.5.1/" "/Applications/Geant4/geant4.9.5-install/lib/Geant4-9.5.0")
else()
  find_package(Geant4 REQUIRED PATHS "/opt/geant4.9.5/lib/Geant4-9.5.1/" "/Applications/Geant4/geant4.9.5-install/lib/Geant4-9.5.0")
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers and macros are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
file(GLOB macros  ${PROJECT_SOURCE_DIR}/*.mac)

#----------------------------------------------------------------------------
# Find Root package
#
#find_package(ROOT REQUIRED)
#if (ROOT_FOUND)
#  include_directories(${ROOT_INCLUDE_DIRS})
#  target_link_libraries (layeredPolymer ${ROOT_LIBRARIES})
#endif (ROOT_FOUND)

if(UNIX)
  execute_process(COMMAND root-config --incdir OUTPUT_VARIABLE ROOT_INC_DIR)
  include_directories(${ROOT_INC_DIR})
  message(STATUS "Should have set root includes to ${ROOT_INC_DIR}")
 #  execute_process(COMMAND root-config --libdir OUTPUT_VARIABLE ROOT_LIB_DIR)
 #  link_directories(${ROOT_LIB_DIR})
   execute_process(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
 #  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")	
   execute_process(COMMAND root-config --libs OUTPUT_VARIABLE ROOT_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
   set(CMAKE_EXE_LINKER_FLAGS ${ROOT_LD_FLAGS})
   message(STATUS "Added ROOT Linking Flags: ${ROOT_LD_FLAGS}")
endif(UNIX)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(layeredPolymer layeredPolymer.cc ${sources} ${headers} ${macros})
target_link_libraries(layeredPolymer ${Geant4_LIBRARIES})
#target_link_libraries(layeredPolymer ${ROOT_LD_FLAGS})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory.
# This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(LAYREDPOLYMER_SCRIPTS
  gui.mac
  init.mac
  init_vis.mac
  run1.mac
  vis.mac
  )

foreach(_script ${LAYREDPOLYMER_SCRIPTS})
  configure_file(${PROJECT_SOURCE_DIR}/${_script} ${PROJECT_BINARY_DIR}/${_script} COPYONLY)
  configure_file(${PROJECT_SOURCE_DIR}/${_script} ${CMAKE_CURRENT_BINARY_DIR}/${_script} COPYONLY)
  file(COPY ${PROJECT_SOURCE_DIR}/${_script} DESTINATION ./)
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS layeredPolymer DESTINATION bin)
