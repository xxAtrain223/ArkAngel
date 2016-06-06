cmake_minimum_required(VERSION 3.2)
project(ArkAngel)

find_package(Git REQUIRED)
find_package(SFML 2 REQUIRED system window graphics audio)
find_package(Boost 1.57.0 REQUIRED)
find_package(Box2D REQUIRED)
find_package(jsoncpp REQUIRED)
add_subdirectory(ginseng)
add_subdirectory(raspberry)
add_subdirectory(bait)
add_subdirectory(print)
add_subdirectory(AngelScript)
add_subdirectory(SFGUI)
add_subdirectory(Thor)
add_subdirectory(b2dJson)

file(GLOB_RECURSE ArkAngel_SOURCES src/*)
list(REMOVE_ITEM ArkAngel_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp")

add_library(_ArkAngel ${ArkAngel_SOURCES})
set_property(TARGET _ArkAngel PROPERTY CXX_STANDARD 14)
set_property(TARGET _ArkAngel APPEND PROPERTY COMPILE_DEFINITIONS BOOST_NO_CXX11_VARIADIC_TEMPLATES)
target_link_libraries(_ArkAngel SFGUI Thor ${SFML_LIBRARIES})
target_link_libraries(_ArkAngel ginseng)
target_link_libraries(_ArkAngel raspberry)
target_link_libraries(_ArkAngel bait)
target_link_libraries(_ArkAngel print)
target_link_libraries(_ArkAngel ${BOX2D_LIBRARIES} b2dJson)
target_link_libraries(_ArkAngel AngelScript
                                AngelScript_scriptbuilder
                                AngelScript_scripthelper
                                AngelScript_scriptstdstring
                                AngelScript_scriptarray
                                AngelScript_scriptdictionary
                                AngelScript_scriptmath)
target_link_libraries(_ArkAngel jsoncpp)
add_definitions(-DSFGUI_STATIC)
add_definitions(-DTHOR_EXPORTS)

add_executable(ArkAngel "src/Main.cpp")
set_property(TARGET ArkAngel PROPERTY CXX_STANDARD 14)
set_property(TARGET ArkAngel APPEND PROPERTY COMPILE_DEFINITIONS BOOST_NO_CXX11_VARIADIC_TEMPLATES)
target_link_libraries(ArkAngel _ArkAngel)
