cmake_minimum_required(VERSION 3.10)

project(ObjectDetection)

set(OpenCV_DIR D:/opencv/opencv/build)
find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp   "src/filter.h" "src/filter.cpp")

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})