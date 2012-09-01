#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(polymerFilm)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#

option(WITH_GEANT4_UI "Build example with Geant4 UI drivers" OFF)
option(WITH_GEANT4_VIS "Build example with Geant4 Vis drivers" ON)
if(WITH_GEANT4_UI)
  find_package(Geant4 REQUIRED ui_all PATHS "/opt/geant4.9.5/lib/Geant4-9.5.1/" "/Applications/Geant4/geant4.9.5-install/lib/Geant4-9.5.0")
else(WITH_GEANT4_VIS)
  find_package(Geant4 REQUIRED vis_all PATHS "/opt/geant4.9.5/lib/Geant4-9.5.1/" "/Applications/Geant4/geant4.9.5-install/lib/Geant4-9.5.0")
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
if(WIN32)
find_package(ROOT REQUIRED)
    if (ROOT_FOUND)
        include_directories(${ROOT_INCLUDE_DIRS})
        target_link_libraries (layeredPolymer ${ROOT_LIBRARIES})
    endif (ROOT_FOUND)
endif(WIN32)
#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(polymerFilm polymerFilm.cc ${sources} ${headers} ${macros})
target_link_libraries(polymerFilm ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(EXAMPLEB2A_SCRIPTS
  gui.mac
  init.mac
  init_vis.mac
  gammaRun.mac
  Co60.mac
  gammaAbsThickness.mac
  neutronRun.mac
  electronRun.mac
  alphaRun.mac
  vis.mac
  )

foreach(_script ${EXAMPLEB2A_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
#install(TARGETS polymerFilm DESTINATION bin)
install(TARGETS polymerFilm DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
