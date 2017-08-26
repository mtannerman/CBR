cmake_minimum_required(VERSION 3.5)


project(CBR)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECTS
	CBRMain)


set(OpenCV_DIR "/home/timarmate/opencv/opencv")
find_package(OpenCV REQUIRED)


FOREACH(project ${PROJECTS})
add_subdirectory(projects/${project})
ENDFOREACH(project)
