cmake_minimum_required(VERSION 3.3)
project(aivctrl
	VERSION 1.0.0
	LANGUAGES C CXX
	DESCRIPTION "Qt5 UPnP C AES256 project"
)

find_package(Qt5 REQUIRED
    Core
    Widgets
    Network
    Xml
)

include_directories(${PROJECT_SOURCE_DIR})
include_directories("${PROJECT_SOURCE_DIR}/../upnp")
message(STATUS "${PROJECT_SOURCE_DIR}/../upnp")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include(aivwidgets/CMakeLists.txt)

list(APPEND AIVCTRL_SOURCES
	mainwindow.cpp
)

list(APPEND AIVCTRL_HEADERS
	mainwindow.hpp
)

list(APPEND AIVCTRL_UIS
	mainwindow.ui
)

list(APPEND AIVCTRL_RES
	aivctrl.qrc
)

if (APPLE)
	set(CMAKE_C_COMPILER /usr/bin/clang)
	set(CMAKE_CXX_COMPILER /usr/bin/clang++)
	set(MACOSX_BUNDLE_ICON_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../qjsonify.icns )
	set(APP_ICON "${MACOSX_BUNDLE_ICON_FILE}")
	list(APPEND OTHER_TARGETS ${APP_ICON})
	list(APPEND AIVCTRL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/cocoainitializer.mm)
	list(APPEND AIVCTRL_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/cocoainitializer.h)
	add_custom_target(OTHER_FILES SOURCES ${OTHER_TARGETS})
	set_property(SOURCE ${APP_ICON} PROPERTY MACOSX_PACKAGE_LOCATION
		${MACOSX_BUNDLE_ICON_FILE})
	file(COPY ${APP_ICON} DESTINATION
		 ${CMAKE_CURRENT_BINARY_DIR})
	add_executable(${PROJECT_NAME} MACOSX_BUNDLE
		${OTHER_FILES}
		${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
		${AIVCTRL_HEADERS}
		${AIVCTRL_SOURCES}
		${AIVCTRL_UIS}
	)
	target_sources(${PROJECT_NAME} PRIVATE
		${OTHER_TARGETS}
	)
	set_source_files_properties(
		${OTHER_TARGETS}
		PROPERTIES
		MACOSX_PACKAGE_LOCATION Resources/
	)
	set_target_properties(${PROJECT_NAME} PROPERTIES
		BUNDLE TRUE
		MACOSX_BUNDLE_GUI_IDENTIFIER org.qtupnp.caes256
		MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
		MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
		MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
	)
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS
		 "-Wl,-F/Library/Frameworks")
	target_link_libraries(${PROJECT_NAME} stdc++
		"-framework Cocoa"
		objc
	)
else()
	add_executable(${PROJECT_NAME}
		main.cpp
		${AIVCTRL_HEADERS}
		${AIVCTRL_SOURCES}
		${AIVCTRL_UIS}
	)
endif()

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets Qt5::Network Qt5::Xml qtupnp)

if(APPLE)
	install(TARGETS ${PROJECT_NAME} BUNDLE DESTINATION ${CMAKE_SOURCE_DIR})
else()
	install(TARGETS ${PROJECT_NAME})
endif()

