cmake_minimum_required(VERSION 2.8.12)

project(listview-generic-cpp-model LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

set(SOURCE_FILES
    main.cpp
    resources.qrc
    cpp/data_item.h
    cpp/data_item.cpp
    cpp/provider.cpp
    cpp/provider.h
    cpp/qobject_list_model.h cpp/data_item_2.cpp cpp/data_item_2.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Quick)
