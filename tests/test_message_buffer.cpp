#include "engine/MessageBuffer.hpp"
#include <iostream>
#include <cassert>
#include <string>
#include <vector>

int main() {
    engine::MessageBuffer buffer;

    // Test 1: Add and flush messages for a single user
    buffer.add_message(123, "Hello");
    buffer.add_message(123, "World");
    auto msgs = buffer.flush_messages(123);
    assert(msgs.size() == 2);
    assert(msgs[0] == "Hello");
    assert(msgs[1] == "World");
    std::cout << "Test 1 passed: Single user add/flush" << std::endl;

    // Test 2: Multiple users
    buffer.add_message(123, "User 1 msg");
    buffer.add_message(456, "User 2 msg");
    buffer.add_message(123, "User 1 msg 2");

    auto msgs1 = buffer.flush_messages(123);
    assert(msgs1.size() == 2);
    assert(msgs1[0] == "User 1 msg");
    assert(msgs1[1] == "User 1 msg 2");

    auto msgs2 = buffer.flush_messages(456);
    assert(msgs2.size() == 1);
    assert(msgs2[0] == "User 2 msg");
    std::cout << "Test 2 passed: Multiple users" << std::endl;

    // Test 3: Clear for user
    buffer.add_message(789, "To be cleared");
    buffer.clear_for_user(789);
    auto msgs3 = buffer.flush_messages(789);
    assert(msgs3.empty());
    std::cout << "Test 3 passed: Clear for user" << std::endl;

    // Test 4: Clear all
    buffer.add_message(1, "A");
    buffer.add_message(2, "B");
    buffer.clear_all();
    assert(buffer.flush_messages(1).empty());
    assert(buffer.flush_messages(2).empty());
    std::cout << "Test 4 passed: Clear all" << std::endl;

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
