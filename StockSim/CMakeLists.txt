cmake_minimum_required (VERSION 3.8)

add_executable (StockSim "StockSim.c" "StockSim.h")

target_include_directories (StockSim PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(StockSim PUBLIC Stock Threads)
