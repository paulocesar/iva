cmake_minimum_required(VERSION 2.6)
	PROJECT(ogre3dtut)
	if (WIN32)
	
		set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
		FILE(GLOB SRCS *.cpp)
		FILE(GLOB HDRS *.h)
		ADD_EXECUTABLE(ogre3dtut
			${SRCS}
			${HDRS}
			)
		
		INCLUDE_DIRECTORIES($(OGRE_HOME)\include)
		LINK_DIRECTORIES(
			$(OGRE_HOME)\lib\${CMAKE_BUILD_TYPE}
			$(OGRE_HOME)\boost_1_42\lib
		)
	
		if("Debug" == ${CMAKE_BUILD_TYPE})
			TARGET_LINK_LIBRARIES(
				ogre3dtut
				OgreMain_d.lib
				OIS_d.lib
				opengl32.lib
				)
		elseif("Release" == ${CMAKE_BUILD_TYPE})
			TARGET_LINK_LIBRARIES(
				ogre3dtut
				OgreMain.lib
				OIS.lib
				opengl32.lib
				)
		endif()
	
	else (WIN32)

	# this was reported to work on linux
		set(CMAKE_MODULE_PATH 
			/usr/local/lib/OGRE/cmake
			)
		FIND_PACKAGE(OpenGL)
		FIND_PACKAGE(OGRE)
		FIND_PACKAGE(OIS)
		
		INCLUDE_DIRECTORIES(
			${OpenGL_INCLUDE_DIR}
			${OGRE_INCLUDE_DIRS}
			${OIS_INCLUDE_DIRS}
			${PROJECT_SOURCE_DIR}/include
			)
		
		FILE(GLOB SRCS *.cpp)
		FILE(GLOB HDRS *.h)
		
		ADD_EXECUTABLE(ogre3dtut 
			${SRCS}
			${HDRS}
		)
		TARGET_LINK_LIBRARIES(ogre3dtut
		${OpenGL_LIBRARIES}
		${OIS_LIBRARIES}
		${OGRE_LIBRARIES}
		)
	endif ()
	