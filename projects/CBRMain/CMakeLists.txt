project(CBRMain)

set(INC_DIR "inc/CBRMain")

set(INC ${INC_DIR}/find_black_squares.h ${INC_DIR}/logging.h ${INC_DIR}/board_rotation_experiment.h ${INC_DIR}/homography.h)
set(SRC src/main.cpp src/find_black_squares.cpp src/logging.cpp src/board_rotation_experiment.cpp src/homography.cpp)

include_directories(inc/CBRMain/)

ADD_EXECUTABLE(CBRMain ${SRC} ${INC})
TARGET_LINK_LIBRARIES(CBRMain ${OpenCV_LIBS})
