add_library(Stock STATIC "Stock.c" "Market.c" "Stock.h" "Market.h")

target_include_directories (LinkedList PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(Stock PUBLIC LinkedList Threads Logger Set)
