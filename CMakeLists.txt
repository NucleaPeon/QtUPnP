cmake_minimum_required(VERSION 3.3)
project(QtUPnP
	VERSION 1.0.0
	LANGUAGES C CXX
	DESCRIPTION "Qt5 UPnP Meta Build"
)

include("upnp/CMakeLists.txt")
include("chupnp/CMakeLists.txt")
