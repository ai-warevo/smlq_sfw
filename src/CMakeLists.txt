add_subdirectory(core)

add_executable(smlq_sfw_tgbog main.cpp)
target_link_libraries(smlq_sfw_tgbog PRIVATE core_lib)
target_include_directories(smlq_sfw_tgbog PRIVATE .)
