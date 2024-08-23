add_executable(logger_ex logger_ex.cpp)
target_link_libraries(logger_ex PRIVATE mgutils)

add_executable(logger_ex2 logger_ex2.cpp)
target_link_libraries(logger_ex2 PRIVATE mgutils)

add_executable(jobpool_ex1 jobpool_ex1.cpp)
target_link_libraries(jobpool_ex1 PRIVATE mgutils)

add_executable(json_comparison json_comparison.cpp)
target_link_libraries(json_comparison PRIVATE mgutils)