cmake_minimum_required(VERSION 3.5)

project(CBR)

MACRO(COPY_DLL trgt lib)
	ADD_CUSTOM_COMMAND (
		TARGET ${trgt}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND}
		ARGS -E copy_if_different "${lib}" $<TARGET_FILE_DIR:${trgt}>
	)
ENDMACRO(COPY_DLL)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set(MSVC_RUNTIME vc14)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Treat warning as error
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")  
endif()


set(PROJECTS
	CBRMain
	math_utils
	common
	imgproc
	recog)


set(OpenCV_DIR dependencies/opencv)
find_package(OpenCV REQUIRED)
MESSAGE(STATUS "OpenCV version (major.minor.patch): ${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}${OpenCV_VERSION_PATCH}")
MESSAGE(STATUS "OpenCV include paths:")
FOREACH(ocvIncDir ${OpenCV_INCLUDE_DIRS})
	MESSAGE(STATUS "                     ${ocvIncDir}")
ENDFOREACH(ocvIncDir)
MESSAGE(STATUS "OpenCV libraries:")
FOREACH(ocvLib ${OpenCV_LIBS})
	MESSAGE(STATUS "                  ${ocvLib}")
ENDFOREACH(ocvLib)


FOREACH(project ${PROJECTS})
add_subdirectory(projects/${project})
ENDFOREACH(project)
