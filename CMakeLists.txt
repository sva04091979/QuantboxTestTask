# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.8)

project ("QuantboxTestTask")

add_subdirectory("Threads")
add_subdirectory("LinkedList")

add_subdirectory("Stock")
add_subdirectory ("StockSim")

#set(CMAKE_C_STANDARD MSVC11)
#set(CMAKE_C_STANDARD_REQUIRED ON)


add_subdirectory("Set")

add_subdirectory("Logger")
