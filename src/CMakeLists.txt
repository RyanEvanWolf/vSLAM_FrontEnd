set(DETECTION_SRCS Detection/FastDetector.cpp)


add_library(vSLAM_FrontEnd ${DETECTION_SRCS})

target_link_libraries(vSLAM_FrontEnd
	${OpenCV_LIBS}
	${Structures_LIBS}
)

