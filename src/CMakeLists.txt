
include_directories(${SFML_INCLUDE_DIR}
		    ${BOX2D_INCLUDE_DIR}
		${EUREKA_INC_DIR})


set(SRC_FILES
	Tetromino.cc
	main.cc)

add_executable(eureka ${SRC_FILES})
target_link_libraries(eureka ${SFML_LIBRARIES} ${BOX2D_LIBRARY})
