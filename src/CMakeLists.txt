
include_directories(${SFML_INCLUDE_DIR})


set(SRC_FILES 
	main.cc)

add_executable(eureka ${SRC_FILES})
target_link_libraries(eureka ${SFML_LIBRARIES})
