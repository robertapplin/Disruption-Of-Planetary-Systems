SET(PROJECT_SRC_DIR "C:/Users/rober/OneDrive/Documents/Visual Studio Projects/DisruptionOfPlanetarySystems/dev")

SET(BOOST_USE_STATIC_LIBS ON)
SET(BOOST_INCLUDEDIR "C:/Program Files/boost/boost_1_55_0")
SET(BOOST_LIBRARYDIR "C:/Program Files/boost/boost_1_55_0/libs")

SET(Qt5_DIR "C:/Qt/5.14.1/msvc2017_64/lib/cmake/Qt5")

SET(ANALYSIS_INC_DIR "disruption-of-planetary-systems/analysis/inc")
SET(INTERFACE_INC_DIR "disruption-of-planetary-systems/_interface/inc")
SET(TOOLS_INC_DIR "tools/inc")

# Libraries must be added before any directory using the library
ADD_SUBDIRECTORY(tools)
ADD_SUBDIRECTORY(disruption-of-planetary-systems)