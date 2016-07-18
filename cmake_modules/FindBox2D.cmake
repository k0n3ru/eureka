

#  BOX2D_FOUND          - True if Box2d was found on your system
#  BOX2D_USE_FILE       - The file making Box2d usable
#  BOX2D_DEFINITIONS    - Definitions needed to build with Box2d
#  BOX2D_INCLUDE_DIR    - Box2d headers location
#  BOX2D_INCLUDE_DIRS   - List of directories where Box2d header file are
#  BOX2D_LIBRARY        - Library name
#  BOX2D_LIBRARIES      - List of libraries to link against
#  BOX2D_LIBRARY_DIRS   - List of directories containing Box2d libraries
#  BOX2D_ROOT_DIR       - The base directory of Box2d
#  BOX2D_VERSION_STRING - A human-readable string containing the version

set( FIND_BOX2D_PATHS
	${BOX2D_ROOT}
	$ENV{BOX2D_ROOT}
	/usr/local
	/usr )

find_path(BOX2D_INCLUDE_DIR Box2D/Box2D.h
	  PATH_SUFFIXES include
	  PATHS ${FIND_BOX2D_PATHS})

find_library ( BOX2D_LIBRARY Box2D
		PATH_SUFFIXES lib lib64
	  	PATHS ${FIND_BOX2D_PATHS})
get_filename_component(BOX2D_LIBRARY_DIR
			${BOX2D_LIBRARY}
			DIRECTORY
			)

