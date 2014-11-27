CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT ( MayaToMantra )

FIND_PACKAGE ( Maya REQUIRED )
FIND_PACKAGE ( Boost REQUIRED COMPONENTS thread )

INCLUDE_DIRECTORIES ( ../common/cpp )
INCLUDE_DIRECTORIES ( ${Boost_INCLUDE_DIR} )

ADD_DEFINITIONS ( ${MAYA_DEFINITIONS} )

ADD_LIBRARY (mayatomantra SHARED
  ./../common/cpp/mayarendernodes/rendercmd.cpp
  ./../common/cpp/mayarendernodes/renderGlobalsNode.cpp
  ./../common/cpp/memory/memoryInfo.cpp
  ./src/mayatomantra.cpp
  ./src/mtm_globalsNode.cpp
  ./src/mtm_envlight.cpp
  ./src/mtm_globals.cpp
  ./src/mtm_mayaObject.cpp
  ./src/mtm_mayaScene.cpp
  ./src/mtm_renderGlobals.cpp
  ./src/mtm_standin.cpp
  ./src/pluginMain.cpp
  ./src/shadingTools/mtm_material.cpp
  ./src/exporter/hsff.cpp
  ./src/exporter/mtm_fluidExporter.cpp
  ./src/exporter/mtm_meshExporter.cpp
  ./src/exporter/mtm_nurbsCurveExporter.cpp
  ./src/exporter/mtm_nurbsExporter.cpp
  ./src/exporter/mtm_particleExporter.cpp
  ./src/exporter/mtm_standinExporter.cpp
  ./../common/cpp/mayaObject.cpp
  ./../common/cpp/mayaScene.cpp
  ./../common/cpp/renderGlobals.cpp
  ./../common/cpp/shadingtools/material.cpp
  ./../common/cpp/shadingtools/shaderDefs.cpp
  ./../common/cpp/shadingtools/shadingNode.cpp
  ./../common/cpp/utilities/attrTools.cpp
  ./../common/cpp/utilities/logging.cpp
  ./../common/cpp/utilities/pystring.cpp
  ./../common/cpp/utilities/tools.cpp
  )
