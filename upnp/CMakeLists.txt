cmake_minimum_required(VERSION 3.3)
project(upnp
	VERSION 1.0.0
	LANGUAGES C CXX
	DESCRIPTION "Qt5 UPnP Library compatible with Qt 5.3.2 & OS X 10.6.8"
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

list(APPEND QTUPNP_HEADERS
    actionmanager.hpp
    controlpoint.hpp
    datacaller.hpp
    devicemap.hpp
    helper.hpp
    initialdiscovery.hpp
    multicastsocket.hpp
    unicastsocket.hpp
    upnpsocket.hpp
    using_upnp_namespace.hpp
    httpparser.hpp
    waitingloop.hpp
    didlitem.hpp
    xmlh.hpp
    xmlhdidllite.hpp
    browsereply.hpp
    connectioninfo.hpp
    transportinfo.hpp
    transportsettings.hpp
    mediainfo.hpp
    positioninfo.hpp
    devicecaps.hpp
    status.hpp
    devicepixmap.hpp
    xmlhdevice.hpp
    device.hpp
    service.hpp
    statevariable.hpp
    action.hpp
    xmlhservice.hpp
    argument.hpp
    avtransport.hpp
    connectionmanager.hpp
    control.hpp
    renderingcontrol.hpp
    contentdirectory.hpp
    avcontrol.hpp
    eventingmanager.hpp
    xmlhevent.hpp
    actioninfo.hpp
    xmlhaction.hpp
    httpserver.hpp
    pixmapcache.hpp
    dump.hpp
    plugin.hpp
    oauth2.hpp
    aesencryption.h
    aes256.h
)

list(APPEND QTUPNP_HEADERS_GLOBAL
	upnp_global.hpp
)

list(APPEND QTUPNP_SOURCES
	actionmanager.cpp
    controlpoint.cpp
    datacaller.cpp
    devicemap.cpp
    helper.cpp
    initialdiscovery.cpp
    multicastsocket.cpp
    unicastsocket.cpp
    upnpsocket.cpp
    httpparser.cpp
    waitingloop.cpp
    didlitem.cpp
    xmlh.cpp
    xmlhdidllite.cpp
    browsereply.cpp
    connectioninfo.cpp
    transportinfo.cpp
    transportsettings.cpp
    mediainfo.cpp
    positioninfo.cpp
    devicecaps.cpp
    devicepixmap.cpp
    xmlhdevice.cpp
    device.cpp
    service.cpp
    statevariable.cpp
    action.cpp
    xmlhservice.cpp
    argument.cpp
    avtransport.cpp
    connectionmanager.cpp
    renderingcontrol.cpp
    contentdirectory.cpp
    avcontrol.cpp
    eventingmanager.cpp
    xmlhevent.cpp
    actioninfo.cpp
    xmlhaction.cpp
    control.cpp
    didlitem_playlist.cpp
    httpserver.cpp
    pixmapcache.cpp
    dump.cpp
    plugin.cpp
    oauth2.cpp
    aesencryption.cpp
)

if(APPLE)
	set(CMAKE_C_COMPILER /usr/bin/clang)
	set(CMAKE_CXX_COMPILER /usr/bin/clang++)
endif()

add_library(${PROJECT_NAME} SHARED
	${QTUPNP_HEADERS}
	${QTUPNP_HEADERS_GLOBAL}
	${QTUPNP_SOURCES}
)
# Captures expected QSysLinux if installed without prefix to /usr
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Network Qt5::Xml)
if (UNIX)
    include_directories(/usr/local/include)
    target_link_libraries(${PROJECT_NAME} QSysLinux )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION}
	SOVERSION 1
)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${QTUPNP_HEADERS_GLOBAL}")
configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)

set_target_properties(
	${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${QTUPNP_HEADERS}")

install(FILES ${header} DESTINATION "${include_dest}")


install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc DESTINATION
	${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

install(TARGETS ${PROJECT_NAME}
	EXPORT ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
)
