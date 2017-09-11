PROJECT(math_utils)


INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(../fw/inc)

set(_src_root_path "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(_inc_root_path "${CMAKE_CURRENT_SOURCE_DIR}/inc/math_utils")

file(
	GLOB_RECURSE _src_list 
	LIST_DIRECTORIES false
	"${_src_root_path}/*.cpp"
)

file(
	GLOB_RECURSE _inc_list 
	LIST_DIRECTORIES false
	"${_inc_root_path}/*.h"    
)


add_library(${PROJECT_NAME} SHARED ${_src_list} ${_inc_list})

foreach(_source IN ITEMS ${_src_list})
get_filename_component(_source_path "${_source}" PATH)
file(RELATIVE_PATH _source_path_rel "${_src_root_path}" "${_source_path}")
string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
source_group("${_group_path}" FILES "${_source}")
endforeach()
foreach(_source IN ITEMS ${_inc_list})
get_filename_component(_source_path "${_source}" PATH)
file(RELATIVE_PATH _source_path_rel "${_inc_root_path}" "${_source_path}")
string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
source_group("${_group_path}" FILES "${_source}")
endforeach()

TARGET_LINK_LIBRARIES(${PROJECT_NAME} fw ${OpenCV_LIBS})
