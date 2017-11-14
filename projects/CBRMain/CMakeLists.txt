project(CBRMain)

SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(INC_DIR "inc/CBRMain")

set(INC )
set(SRC src/main.cpp)

include_directories(inc/CBRMain/)
include_directories(../common/inc)
include_directories(../imgproc/inc)
include_directories(../recog/inc)
include_directories(../math_utils/inc)
include_directories(../cbr_test/inc)

ADD_EXECUTABLE(CBRMain ${SRC} ${INC})
set(CBRMainTargets "")
LIST(APPEND CBRMainTargets common)
LIST(APPEND CBRMainTargets imgproc)
LIST(APPEND CBRMainTargets recog)
LIST(APPEND CBRMainTargets math_utils)
LIST(APPEND CBRMainTargets cbr_test)
TARGET_LINK_LIBRARIES(CBRMain ${CBRMainTargets} ${OpenCV_LIBS})

# OpenCV
SET(COPY_OpenCV_LIBS opencv_highgui opencv_core opencv_imgproc opencv_imgcodecs opencv_flann opencv_features2d opencv_ml opencv_objdetect opencv_calib3d opencv_video opencv_videoio)

INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(CBRMain ${OpenCV_LIBS})

SET(OpenCV_LIB_POST ${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}${OpenCV_VERSION_PATCH})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set(BUILD_TYPE_STR "Debug")
	set(CBRMAIN_COPY_SRC_DIR "${CBR_DIRECTORY}/build/projects")
	FOREACH(trgt ${CBRMainTargets})
		COPY_DLL(${PROJECT_NAME} "${CBRMAIN_COPY_SRC_DIR}/${trgt}/${BUILD_TYPE_STR}/${trgt}.dll")
	ENDFOREACH(trgt)
	
	FOREACH(lib ${COPY_OpenCV_LIBS})
		COPY_DLL(${PROJECT_NAME} "${OpenCV_DIR}/bin/Release/${lib}${OpenCV_LIB_POST}.dll")
		COPY_DLL(${PROJECT_NAME} "${OpenCV_DIR}/bin/Debug/${lib}${OpenCV_LIB_POST}d.dll")
	ENDFOREACH(lib)
endif()
