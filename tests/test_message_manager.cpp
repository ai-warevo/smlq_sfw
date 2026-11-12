#include "engine/MessageManager.hpp"
#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

int main() {
    bool flushed = false;
    std::int64_t last_flushed_user = 0;
    std::vector<std::string> last_flushed_msgs;

    engine::MessageManager manager([&](std::int64_t user_id, std::vector<std::string> msgs) {
        flushed = true;
        last_flushed_user = user_id;
        last_flushed_msgs = std::move(msgs);
    }, std::chrono::seconds(1));

    // Test 1: Immediate activity
    manager.addMessage(123, "Msg 1");
    manager.addMessage(123, "Msg 2");
    
    std::cout << "Waiting for silence window (1s)..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    assert(flushed);
    assert(last_flushed_user == 123);
    assert(last_flushed_msgs.size() == 2);
    assert(last_flushed_msgs[0] == "Msg 1");
    assert(last_flushed_msgs[1] == "Msg 2");
    std::cout << "Test 1 passed: Silence window flush" << std::endl;

    // Test 2: Resetting timer with new activity
    flushed = false;
    manager.addMessage(456, "Msg A");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    manager.addMessage(456, "Msg B"); // Should reset timer
    
    std::cout << "Waiting for silence window (1s)..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(700));
    assert(!flushed); // Should not have flushed yet because of Msg B

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    assert(flushed);
    assert(last_flushed_user == 456);
    assert(last_flushed_msgs.size() == 2);
    std::cout << "Test 2 passed: Timer reset on activity" << std::endl;

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
