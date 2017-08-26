project(CBRMain)
set(INC "")
set(SRC src/main.cpp)

ADD_EXECUTABLE(CBRMain ${SRC} ${INC})
TARGET_LINK_LIBRARIES(CBRMain ${OpenCV_LIBS})
