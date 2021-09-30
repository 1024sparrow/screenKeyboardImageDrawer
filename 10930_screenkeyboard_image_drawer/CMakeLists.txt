cmake_minimum_required(VERSION 2.8)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5 REQUIRED COMPONENTS
	Widgets
)

#set(SOURCES
#	WIN32 main.cpp
#)
file(GLOB SOURCES *.cpp)
#qt5_add_resources (RCC_SOURCES rc.qrc)
add_executable(10930_screenkeyboard_image_drawer ${SOURCES} ${RCC_SOURCES})
target_link_libraries(10930_screenkeyboard_image_drawer Qt5::Core
	Qt5::Widgets
	#$<$<PLATFORM_ID:Linux>:pthread>
)

