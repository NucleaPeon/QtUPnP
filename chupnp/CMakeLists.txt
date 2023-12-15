cmake_minimum_required(VERSION 3.3)
project(chupnp
	VERSION 1.0.0
	LANGUAGES C CXX
	DESCRIPTION "Qt5 UPnP Developer Project"
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
include(GNUInstallDirs)  # REQUIRED TO INSTALL PKGCONFIG TO PROPER DIR!
#add_compile_options(-v)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

list(APPEND CHUPNP_SOURCES
	actionParams.cpp
	actiontriggered.cpp
	helper.cpp
	item.cpp
	loadservices.cpp
	mainwindow.cpp
	search.cpp
	sendgetaction.cpp
	slots.cpp
	stdaction.cpp
)

list(APPEND CHUPNP_HEADERS
	actionParams.hpp
	helper.hpp
	item.hpp
	mainwindow.hpp
	search.hpp
	stdaction.hpp
)

list(APPEND CHUPNP_UIS
	search.ui
	stdaction.ui
	mainwindow.ui
	actionParams.ui
)

list(APPEND CHUPNP_RES
	chupnp.qrc
)

if (APPLE)
	set(CMAKE_C_COMPILER /usr/bin/clang)
	set(CMAKE_CXX_COMPILER /usr/bin/clang++)

	set(MACOSX_BUNDLE_ICON_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../qjsonify.icns )
	set(APP_ICON "${MACOSX_BUNDLE_ICON_FILE}")
	list(APPEND OTHER_TARGETS ${APP_ICON})
	list(APPEND CHUPNP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/cocoainitializer.mm)
	list(APPEND CHUPNP_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/cocoainitializer.h)
	add_custom_target(OTHER_FILES SOURCES ${OTHER_TARGETS})
	set_property(SOURCE ${APP_ICON} PROPERTY MACOSX_PACKAGE_LOCATION
		${MACOSX_BUNDLE_ICON_FILE})
	file(COPY ${APP_ICON} DESTINATION
		 ${CMAKE_CURRENT_BINARY_DIR})
	add_executable(${PROJECT_NAME} MACOSX_BUNDLE
		${OTHER_FILES}
		${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
		${CHUPNP_HEADERS}
		${CHUPNP_SOURCES}
		${CHUPNP_UIS}
		${CHUPNP_RES}
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
		MACOSX_BUNDLE_GUI_IDENTIFIER org.qtupnp.chupnp
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
		${OTHER_FILES}
		main.cpp
		${CHUPNP_HEADERS}
		${CHUPNP_SOURCES}
		${CHUPNP_UIS}
		${CHUPNP_RES}
	)
endif()

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets Qt5::Network Qt5::Xml qtupnp)

if(APPLE)
	install(TARGETS ${PROJECT_NAME} BUNDLE DESTINATION ${CMAKE_SOURCE_DIR})
else()
	install(TARGETS ${PROJECT_NAME})
endif()
