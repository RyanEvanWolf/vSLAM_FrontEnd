set(DETECTION_SRCS 
Detection/FastDetector.cpp
Detection/SiftDetector.cpp
Detection/OrbDetector.cpp
Detection/AkazeDetector.cpp
Detection/KazeDetector.cpp
Detection/ASiftDetector.cpp
)

set(STEREO_FRONT_SRCS
StereoFront/StereoCamera.cpp
StereoFront/Matching.cpp
)

add_library(vSLAM_FrontEnd ${DETECTION_SRCS} ${STEREO_FRONT_SRCS})

MESSAGE(STATUS "${KAZE_LIBS} --- ${KAZE_LIBRARIES_DIR} ---${KAZE_INCLUDE_DIRS}")
target_link_libraries(vSLAM_FrontEnd
	${OpenCV_LIBS}
	${Structures_LIBS}
	${AKAZE_LIBS}
	${KAZE_LIBS}
)
add_executable(saveFastDataset ${CMAKE_CURRENT_LIST_DIR}/bin/saveFastDataset.cpp)
target_link_libraries(saveFastDataset vSLAM_FrontEnd)

add_executable(saveSiftDataset ${CMAKE_CURRENT_LIST_DIR}/bin/saveSiftDataset.cpp)
target_link_libraries(saveSiftDataset vSLAM_FrontEnd)

#add_executable(saveBriskDataset ${CMAKE_CURRENT_LIST_DIR}/bin/saveBriskDataset.cpp)
#target_link_libraries(saveBriskDataset vSLAM_FrontEnd)

add_executable(saveOrbDataset ${CMAKE_CURRENT_LIST_DIR}/bin/saveOrbDataset.cpp)
target_link_libraries(saveOrbDataset vSLAM_FrontEnd)


add_executable(displayAKAZE ${CMAKE_CURRENT_LIST_DIR}/bin/displayAKAZE.cpp)
target_link_libraries(displayAKAZE vSLAM_FrontEnd)


add_executable(displayKAZE ${CMAKE_CURRENT_LIST_DIR}/bin/displayKAZE.cpp)
target_link_libraries(displayKAZE vSLAM_FrontEnd)