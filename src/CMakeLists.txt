set(DETECTION_SRCS 
Detection/FastDetector.cpp
Detection/SiftDetector.cpp
Detection/ASiftDetector.cpp
Detection/KazeDetector.cpp
Detection/BriskDetector.cpp
#Detection/OrbDetector.cpp
#Detection/FreakDetector.cpp
)
set(DESCRIPTION_SRCS Description/BriskDescription.cpp)
set(STEREO_FRONT_SRCS StereoFront/StereoCamera.cpp)


add_library(vSLAM_FrontEnd ${DETECTION_SRCS} ${DESCRIPTION_SRCS} ${STEREO_FRONT_SRCS})

target_link_libraries(vSLAM_FrontEnd
	${OpenCV_LIBS}
	${Structures_LIBS}
	KAZE
)



add_executable(saveFastDataset ${CMAKE_CURRENT_LIST_DIR}/bin/saveFastDataset.cpp)
target_link_libraries(saveFastDataset vSLAM_FrontEnd)

add_executable(saveSiftDataset ${CMAKE_CURRENT_LIST_DIR}/bin/saveSiftDataset.cpp)
target_link_libraries(saveSiftDataset vSLAM_FrontEnd)

add_executable(saveBriskDataset ${CMAKE_CURRENT_LIST_DIR}/bin/saveBriskDataset.cpp)
target_link_libraries(saveBriskDataset vSLAM_FrontEnd)

#add_executable(saveOrbDataset ${CMAKE_CURRENT_LIST_DIR}/bin/saveOrbDataset.cpp)
#target_link_libraries(saveOrbDataset vSLAM_FrontEnd)

#add_executable(saveFreakDataset ${CMAKE_CURRENT_LIST_DIR}/bin/saveFreakDataset.cpp)
#target_link_libraries(saveFreakDataset vSLAM_FrontEnd)