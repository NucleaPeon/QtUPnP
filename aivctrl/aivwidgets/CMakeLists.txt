cmake_minimum_required(VERSION 3.3)
project(upnpwidgets
	VERSION 1.0.0
	LANGUAGES C CXX
	DESCRIPTION "Qt5 UPnP Widget Library"
)

find_package(Qt5 REQUIRED
    Core
    Gui
    Network
    Xml
)

include(GNUInstallDirs)  # REQUIRED TO INSTALL PKGCONFIG TO PROPER DIR!
#add_compile_options(-v)
set(CMAKE_AUTOMOC ON)


list(APPEND QTUPNPW_HEADERS_GLOBAL

)

list(APPEND QTUPNPW_HEADERS
    ${QTUPNPW_HEADERS_GLOBAL}
    cloudbrowser.hpp
    contentdirectorybrowser.hpp
    cover.hpp
    devicebrowser.hpp
    folderbrowser.hpp
    listwidgetbase.hpp
    mydevicebrowser.hpp
    networkprogress.hpp
    playlistbrowser.hpp
    position.hpp
    slider.hpp
    textmetadata.hpp
    widgethelper.hpp
)


list(APPEND QTUPNPW_SOURCES
    cloudbrowser.cpp
    contentdirectorybrowser.cpp
    cover.cpp
    devicebrowser.cpp
    folderbrowser.cpp
    listwidgetbase.cpp
    mydevicebrowser.cpp
    networkprogress.cpp
    playlistbrowser.cpp
    position.cpp
    slider.cpp
    textmetadata.cpp
    widgethelper.cpp
)

if(APPLE)
	set(CMAKE_C_COMPILER /usr/bin/clang)
	set(CMAKE_CXX_COMPILER /usr/bin/clang++)
endif()

add_library(${PROJECT_NAME} STATIC
	${QTUPNPW_HEADERS}
	${QTUPNPW_HEADERS_GLOBAL}
	${QTUPNPW_SOURCES}
)
# Captures expected QSysLinux if installed without prefix to /usr
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Network Qt5::Xml)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION}
	SOVERSION 1
)
# set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${QTUPNPW_HEADERS_GLOBAL}")
configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)

set_target_properties(
	${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${QTUPNPW_HEADERS}")

install(FILES ${header} DESTINATION "${include_dest}")


install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc DESTINATION
	${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

install(TARGETS ${PROJECT_NAME}
	EXPORT ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
)
