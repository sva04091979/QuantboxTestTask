add_library(LinkedList STATIC "LinkedList.c" "LinkedList.h")

target_include_directories (LinkedList PUBLIC ${PROJECT_SOURCE_DIR})
