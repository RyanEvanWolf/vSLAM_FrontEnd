set(DETECTION_SRCS 
Detection/FastDetector.cpp
Detection/SiftDetector.cpp
Detection/ASiftDetector.cpp
)
set(DESCRIPTION_SRCS Description/BriskDescription.cpp)
set(STEREO_FRONT_SRCS StereoFront/StereoCamera.cpp)


add_library(vSLAM_FrontEnd ${DETECTION_SRCS} ${DESCRIPTION_SRCS} ${STEREO_FRONT_SRCS})

target_link_libraries(vSLAM_FrontEnd
	${OpenCV_LIBS}
	${Structures_LIBS}
)

add_executable(saveDataset ${CMAKE_CURRENT_LIST_DIR}/bin/saveDataset.cpp)
target_link_libraries(saveDataset vSLAM_FrontEnd)

add_executable(saveFastDataset ${CMAKE_CURRENT_LIST_DIR}/bin/saveFastDataset.cpp)
target_link_libraries(saveFastDataset vSLAM_FrontEnd)

add_executable(saveSiftDataset ${CMAKE_CURRENT_LIST_DIR}/bin/saveSiftDataset.cpp)
target_link_libraries(saveSiftDataset vSLAM_FrontEnd)