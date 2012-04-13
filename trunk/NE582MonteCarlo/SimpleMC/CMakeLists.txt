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
# Add the executable, and link it to the Math Library
#
add_executable(simpleMC simpleMC.c ${sources} ${headers})
add_executable(tallyTest tallyTest.c ${sources} ${headers})
target_link_libraries(simpleMC)


#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS simpleMC DESTINATION bin)
