cmake_minimum_required(VERSION 3.10.0)

project(DEngine VERSION 1.3.5 LANGUAGES C CXX)

option(DENGINE_COMPILE_SAMPLES "Compile samples of DEngine" ON)
option(BUILD_SHARED_LIBS "Compile shared libs" ON)
file(GLOB DENGINE_SOURCES "src/*.cpp" "src/miniaudio.c")
file(GLOB DENGINE_HEADERS "include/*.h")

if(WIN32)
	message(STATUS "Compiling for Windows")
	add_library(DEngine ${DENGINE_SOURCES} ${DENGINE_HEADERS} version.rc)
	target_compile_definitions(DEngine PRIVATE GLEW_BUILD GLEW_STATIC)

	target_include_directories(DEngine
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/glm"
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/glew-2.3.1/include"
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/glfw-3.4.bin.WIN64/include")

	target_link_libraries(DEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/glew-2.3.1/lib/libglew32d.a"
	"${CMAKE_CURRENT_SOURCE_DIR}/glfw-3.4.bin.WIN64/lib/libglfw3.a" opengl32 gdi32 ws2_32)
elseif(ANDROID)
	message(STATUS "Compiling for Android")
	if(${DENGINE_COMPILE_SAMPLES})
		message(WARNING "Disabling compiling of samples. Doesn't supported on Android")
		set(DENGINE_COMPILE_SAMPLES OFF)
	endif()
	add_library(DEngine ${DENGINE_SOURCES} ${DENGINE_HEADERS})

	target_include_directories(DEngine
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/glm")
	
	find_library(gles3-lib GLESv3 REQUIRED)
	find_library(log-lib log REQUIRED)
	find_library(android-lib android REQUIRED)
	target_link_libraries(DEngine ${gles3-lib} ${log-lib} ${android-lib})
else()
	message(FATAL_ERROR "Platform not supported yet.")
endif()
# Samples
if(${DENGINE_COMPILE_SAMPLES})
	# Samples
	## Texture cube
	add_executable(textureCube samples/textureCube.cpp)
	target_link_libraries(textureCube DEngine)
	## Sound in 3D
	add_executable(sound_in_3d samples/sound_in_3d.cpp)
	target_link_libraries(sound_in_3d DEngine)
	## Alpha demo
	add_executable(alpha_demo samples/alpha_demo.cpp)
	target_link_libraries(alpha_demo DEngine)
	## .obj loading
	add_executable(obj_loading samples/obj_loading.cpp)
	target_link_libraries(obj_loading DEngine)
	## Shadow
	add_executable(shadow samples/shadow.cpp)
	target_link_libraries(shadow DEngine)
	## Gui sample
	add_executable(gui_sample samples/gui_sample.cpp)
	target_link_libraries(gui_sample DEngine)
	file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/samples/res" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endif()