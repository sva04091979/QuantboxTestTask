add_library(Threads STATIC "Threads.c" "Threads.h")

target_include_directories (Threads PUBLIC ${PROJECT_SOURCE_DIR})
