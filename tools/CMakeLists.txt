SET(
  INC_FILES
  inc/FileManager.h
  inc/Logger.h
  inc/PerformanceChecker.h
  inc/ThreadPool.h
)

SET(
  SRC_FILES
  src/FileManager.cpp
  src/Logger.cpp
  src/PerformanceChecker.cpp
  src/ThreadPool.cpp
)

FIND_PACKAGE(Boost 1.55.0)
FIND_PACKAGE(Qt5 COMPONENTS Core Gui Widgets)

INCLUDE_DIRECTORIES(${BOOST_INCLUDEDIR})
INCLUDE_DIRECTORIES(${PROJECT_SRC_DIR}/${TOOLS_INC_DIR})

ADD_LIBRARY(Tools ${INC_FILES} ${SRC_FILES})

TARGET_LINK_LIBRARIES(Tools PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets)

TARGET_INCLUDE_DIRECTORIES(Tools PUBLIC ${PROJECT_SRC_DIR}/${TOOLS_INC_DIR})