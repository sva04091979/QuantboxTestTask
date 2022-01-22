# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.8)

project ("QuantboxTestTask")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_subdirectory("Threads")
add_subdirectory("LinkedList")

add_subdirectory("Stock")
add_subdirectory ("StockSim")

add_subdirectory("Set")

add_subdirectory("Logger")

