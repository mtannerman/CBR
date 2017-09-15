project(CBRMain)

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
