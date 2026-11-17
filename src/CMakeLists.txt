add_executable(smlq_sfw_tgbog main.cpp td_client.cpp)
target_link_libraries(smlq_sfw_tgbog PRIVATE Td::TdStatic)
target_include_directories(smlq_sfw_tgbog PRIVATE ${CMAKE_SOURCE_DIR}/include .)
