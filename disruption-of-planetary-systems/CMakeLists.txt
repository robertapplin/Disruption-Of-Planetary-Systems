SET(
  INC_FILES
  analysis/inc/Body.h
  analysis/inc/GenerateInitFiles.h
  analysis/inc/ProcessOutFiles.h
  analysis/inc/SimParameters.h
  analysis/inc/SimResults.h
  analysis/inc/SimulateInitFiles.h
  _interface/inc/DPSInterface.h
  _interface/inc/DPSInterfaceModel.h
  _interface/inc/DPSInterfacePresenter.h
  _interface/inc/DPSInterfaceView.h
)

SET(
  SRC_FILES
  analysis/src/Body.cpp
  analysis/src/GenerateInitFiles.cpp
  analysis/src/ProcessOutFiles.cpp
  analysis/src/SimParameters.cpp
  analysis/src/SimResults.cpp
  analysis/src/SimulateInitFiles.cpp
  _interface/src/DPSInterface.cpp
  _interface/src/DPSInterfaceModel.cpp
  _interface/src/DPSInterfacePresenter.cpp
  _interface/src/DPSInterfaceView.cpp
  _interface/src/main.cpp
)

SET(
  UI_FILES
  _interface/inc/DPSInterface.ui
  _interface/inc/DPSInterfaceView.ui
)

#ADD_DEFINITIONS(-DBOOST_ALL_NO_LIB) # Disable interference of find_package(Boost) with auto-linking
#SET(Boost_USE_STATIC_LIBS ON) # Link to static versions

SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)

FIND_PACKAGE(Boost 1.55.0)
FIND_PACKAGE(Qt5 COMPONENTS Core Gui Widgets)

INCLUDE_DIRECTORIES(SHARED ${BOOST_INCLUDEDIR})
INCLUDE_DIRECTORIES(${PROJECT_SRC_DIR}/${ANALYSIS_INC_DIR})
INCLUDE_DIRECTORIES(${PROJECT_SRC_DIR}/${INTERFACE_INC_DIR})

ADD_EXECUTABLE(${PROJECT_NAME} ${INC_FILES} ${SRC_FILES})

#LINK_LIBRARIES(${PROJECT_NAME} ${BOOST_LIBRARYDIR})  # Keep this
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets Tools)