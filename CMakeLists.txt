cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(clientFilemanager LANGUAGES CXX)
add_executable(clientFilemanager utils.h utils.cpp clientFilemanager.cpp filemanager_stub.h operaciones.h)
target_link_libraries(clientFilemanager pthread)

project(serverFilemanager LANGUAGES CXX)
add_executable(serverFilemanager utils.h utils.cpp serverFilemanager.cpp filemanager.h filemanager.cpp filemanager_imp.h operaciones.h)
target_link_libraries(serverFilemanager pthread)