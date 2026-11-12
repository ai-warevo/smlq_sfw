add_library(core_lib STATIC 
    td_example.cpp 
    ../engine/UpdateDispatcher.cpp
)
target_include_directories(core_lib PUBLIC . ..)
target_link_libraries(core_lib PUBLIC Td::TdStatic)
