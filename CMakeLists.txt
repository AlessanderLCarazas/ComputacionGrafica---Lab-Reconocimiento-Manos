cmake_minimum_required(VERSION 3.30.0)

project(CG - Lab4Manos)

set(OpenCV_DIR D:/Programming Languages/OpenCV/opencv/build)
find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME} src/Main.cpp)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})