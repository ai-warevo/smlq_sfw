#include "core/td_example.hpp"
#include <atomic>
#include <csignal>

std::atomic<bool> is_running{true};

void signal_handler(int signal) {
  is_running = false;
}

int main() {
  std::signal(SIGINT, signal_handler);
  std::signal(SIGTERM, signal_handler);

  TdExample example;
  example.loop();
  example.shutdown();
}
