set(CMAKE_CXX_STANDARD 20)

add_library(classes Background.cpp Crosshair.cpp Duck.cpp)

target_include_directories(classes INTERFACE
                          ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(classes PUBLIC raylib)

target_include_directories(classes PUBLIC 
                            "${CMAKE_CURRENT_LIST_DIR}/..")