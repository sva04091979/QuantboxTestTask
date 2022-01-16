add_library(Logger "Logger.c" "Logger.h")

target_include_directories (Logger PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(Logger PUBLIC LinkedList Threads)
