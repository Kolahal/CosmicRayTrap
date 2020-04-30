#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(MMT)

#----------------------------------------------------------------------------
# Add ROOT
# Check, if ROOTSYS is set.
if(NOT DEFINED ROOTSYS)
  set(ROOTSYS $ENV{ROOTSYS})
  if(NOT DEFINED ROOTSYS)
    message(FATAL_ERROR "ROOTSYS is not set!")
  endif()
endif()

if(EXISTS $ENV{ROOTSYS}/cmake/ROOTConfig.cmake)
  list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
else()
  list(APPEND CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)
endif()

find_package(ROOT REQUIRED COMPONENTS RIO Net)
#include(${ROOT_USE_FILE})

#set(CMAKE_MODULE_PATH /people/bhat731/Package/ROOT/root5.36/root/etc/cmake/FindROOT.cmake)
#include(/people/bhat731/Package/ROOT/root5.36/root/etc/cmake/FindROOT.cmake)
#----------------------------------------------------------------------------
# $Id: CMakeLists.txt 86065 2014-11-07 08:51:15Z gcosmo $

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
 
endif()
#find_package(Geant4 REQUIRED ui_all vis_all)
find_package( PythonInterp 3 REQUIRED )
find_package( PythonLibs 3 REQUIRED )

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
#find_package(ROOT REQUIRED COMPONENTS RIO Net)

#endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${ROOT_INCLUDE_DIRS})


#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(mmt mmtMain.cc ${sources} ${headers})

target_link_libraries(mmt ${PYTHON_LIBRARIES})

#link_directories(/people/bhat731/CosmicMuTrap/CRY/cry_v1.7/lib/)
#target_link_libraries(mmt libCRY.a )


#link_directories($(CRYHOME)/lib)
target_link_libraries(mmt /home/kolahalb/Works/Computation/Packages/CRY/cry_v1.7/lib/libCRY.a)

target_link_libraries(mmt ${Geant4_LIBRARIES})

target_link_libraries(mmt ${ROOT_LIBRARIES})
target_link_libraries(mmt ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#

set(EXAMPLE_MMT_SCRIPTS
  #exampleB1.in
  #exampleB1.out
  init_vis.mac
  run1.mac
  run2.mac
  vis.mac
  )

foreach(_script ${EXAMPLE_MMT_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()
#set(EXAMPLE_MMT_SCRIPTS
#  exampleB1.in
#  exampleB1.out
#  init_vis.mac
#  run1.mac
#  run2.mac
#  vis.mac
#  )
#
#foreach(_script ${EXAMPLEB1_SCRIPTS})
#  configure_file(
#    ${PROJECT_SOURCE_DIR}/${_script}
#    ${PROJECT_BINARY_DIR}/${_script}
#    COPYONLY
#    )
#endforeach()

#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
add_custom_target(MMT DEPENDS mmt)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS mmt DESTINATION bin)


