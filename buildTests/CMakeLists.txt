cmake_minimum_required(VERSION 3.2)
project(BuildTests)

add_executable(Console buildTests/Console.cpp)

find_package(SFML REQUIRED system window graphics)
add_executable(Sfml buildTests/SFML.cpp)
target_link_libraries(Sfml ${SFML_LIBRARIES})
set_property(TARGET Sfml PROPERTY CXX_STANDARD 14)

add_subdirectory(SFGUI)
add_executable(Sfgui buildTests/SFGUI.cpp)
add_definitions(-DSFGUI_STATIC)
target_link_libraries(Sfgui SFGUI ${SFML_LIBRARIES})
set_property(TARGET Sfgui PROPERTY CXX_STANDARD 14)

add_subdirectory(ginseng)
add_executable(Ginseng buildTests/Ginseng.cpp)
target_link_libraries(Ginseng ginseng)
set_property(TARGET Ginseng PROPERTY CXX_STANDARD 14)

add_subdirectory(raspberry)
add_executable(Raspberry buildTests/Raspberry.cpp)
target_link_libraries(Raspberry raspberry)
set_property(TARGET Raspberry PROPERTY CXX_STANDARD 14)

add_subdirectory(bait)
add_executable(Bait buildTests/Bait.cpp)
target_link_libraries(Bait bait)
set_property(TARGET Bait PROPERTY CXX_STANDARD 14)

find_package(Box2D REQUIRED)
add_executable(box2D buildTests/Box2D.cpp)
target_link_libraries(box2D ${BOX2D_LIBRARIES})
set_property(TARGET box2D PROPERTY CXX_STANDARD 14)

add_subdirectory(AngelScript)
add_executable(Angelscript buildTests/AngelScript.cpp)
target_link_libraries(Angelscript AngelScript AngelScript_scriptstdstring winmm)
set_property(TARGET Angelscript PROPERTY CXX_STANDARD 14)
configure_file(buildTests/script.as script.as COPYONLY)

find_package(jsoncpp REQUIRED)
add_executable(Jsoncpp buildTests/Json.cpp)
target_link_libraries(Jsoncpp jsoncpp)
set_property(TARGET Jsoncpp PROPERTY CXX_STANDARD 14)
