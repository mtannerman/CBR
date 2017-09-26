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

ADD_EXECUTABLE(CBRMain ${SRC} ${INC})
TARGET_LINK_LIBRARIES(CBRMain ${OpenCV_LIBS} common imgproc recog math_utils)

# OpenCV
SET(COPY_OpenCV_LIBS opencv_highgui opencv_core opencv_imgproc opencv_imgcodecs opencv_flann opencv_features2d opencv_ml opencv_objdetect opencv_calib3d opencv_video opencv_videoio)

INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(CBRMain ${OpenCV_LIBS})

SET(OpenCV_LIB_POST ${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}${OpenCV_VERSION_PATCH})

MESSAGE(STATUS "OpenCV_LIB_PATH: ${OpenCV_LIB_PATH}")

FOREACH(lib ${COPY_OpenCV_LIBS})
	COPY_DLL(${PROJECT_NAME} "${OpenCV_DIR}/bin/Release/${lib}${OpenCV_LIB_POST}.dll")
	COPY_DLL(${PROJECT_NAME} "${OpenCV_DIR}/bin/Debug/${lib}${OpenCV_LIB_POST}d.dll")
	#COPY_DLL(${PROJECT_NAME} "${OpenCV_LIB_PATH}/../bin/${lib}${OpenCV_LIB_POST}d.dll")
	#COPY_DLL(${PROJECT_NAME} "${OpenCV_LIB_PATH}/../bin/${lib}${OpenCV_LIB_POST}.dll")
	#LIST(APPEND EXTERNAL_DLLS "${OpenCV_LIB_PATH}/../bin/${lib}${OpenCV_LIB_POST}.dll")
ENDFOREACH(lib)
