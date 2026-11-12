add_library(core_lib STATIC 
    td_example.cpp 
    ../engine/UpdateDispatcher.cpp
    ../engine/PresenceSimulator.cpp
    ../engine/MessageBuffer.cpp
    ../engine/SilenceWindow.cpp
    ../engine/MessageManager.cpp
    ../engine/WorkerPool.cpp
)
target_include_directories(core_lib PUBLIC . ..)
target_link_libraries(core_lib PUBLIC Td::TdStatic)
