cmake_minimum_required(VERSION 2.8.12)

project(listview-generic-cpp-model LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp
    resources.qrc
    cpp/data_item.h
    cpp/data_item.cpp
    cpp/provider.cpp
    cpp/provider.h
    cpp/list_model.cpp
    cpp/list_model.h)

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Quick)
