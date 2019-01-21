cmake_minimum_required(VERSION 2.8)
PROJECT(TULANE_EARTH)
SET(CMAKE_BUILD_TYPE "Release")

if (!MSVC)
SET(CMAKE_CXX_FLAGS "-Wno-deprecated")
endif()

#Compile and Link GLFW
ADD_SUBDIRECTORY(glfw-3.2)
link_libraries(glfw)
include_directories(${glfw_INCLUDE_DIRS})
include_directories("${CMAKE_SOURCE_DIR}/glfw-3.2/deps")

add_library(glad "${CMAKE_SOURCE_DIR}/glfw-3.2/deps/glad/glad.h"
         		 "${CMAKE_SOURCE_DIR}/glfw-3.2/deps/glad.c")
				 
link_libraries(glad)

SUBDIRS(${PROJECT_SOURCE_DIR}/FreeImage3151)	

INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/FreeImage3151/Source")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/FreeImage3151/Source/LibJPEG")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/FreeImage3151/Source/LibMNG")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/FreeImage3151/Source/LibOpenJPEG")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/FreeImage3151/Source/LibPNG/")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/FreeImage3151/Source/LibRawLite")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/FreeImage3151/Source/LibTIFF")

SET(MY_SOURCE_PATH ${CMAKE_SOURCE_DIR})
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/source/common/SourcePath.cpp.in ${CMAKE_SOURCE_DIR}/source/common/SourcePath.cpp)	

include_directories(${CMAKE_SOURCE_DIR}/source/common
					${CMAKE_SOURCE_DIR}/shaders)
add_executable(earth WIN32 MACOSX_BUNDLE 
	source/earth.cpp 
	source/common/common.h
	source/common/CheckError.h
	source/common/mat.h
	source/common/ObjMesh.cpp
	source/common/ObjMesh.h
	source/common/SourcePath.cpp
	source/common/SourcePath.h
	source/common/Trackball.cpp
	source/common/Trackball.h
	source/common/vec.h
	shaders/fshader.glsl
   shaders/vshader.glsl)

ADD_DEFINITIONS(-DFREEIMAGE_LIB=1)
	
TARGET_LINK_LIBRARIES(earth FreeImage)

#Windows cleanup
if (MSVC)
    # Tell MSVC to use main instead of WinMain for Windows subsystem executables
    set_target_properties(earth PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()

#Apple cleanup
if (APPLE)
    set_target_properties(earth PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "simple")
	
    set_target_properties(earth PROPERTIES
                          MACOSX_BUNDLE_SHORT_VERSION_STRING ${GLFW_VERSION}
                          MACOSX_BUNDLE_LONG_VERSION_STRING ${GLFW_VERSION_FULL}
    						  	  MACOSX_BUNDLE_ICON_FILE glfw.icns)				  
endif()
