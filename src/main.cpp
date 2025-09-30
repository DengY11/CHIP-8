#include <chrono>
#include <iostream>
#include <thread>

#include "chip8.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <ROM file>" << std::endl;
        return 1;
    }

    CHIP8::Chip8CPU cpu;
    if (!cpu.loadROM(argv[1])) {
        std::cerr << "Failed to load ROM: " << argv[1] << std::endl;
        return 1;
    }
    using clock = std::chrono::high_resolution_clock;
    auto last_cycle_time = clock::now();
    auto last_timer_time = clock::now();

    const std::chrono::duration<double> cpu_cycle_duration(1.0 /
                                                           500.0);   // 500 Hz
    const std::chrono::duration<double> timer_duration(1.0 / 60.0);  // 60 Hz

    while (cpu.handle_input()) {
        auto current_time = clock::now();

        if (current_time - last_cycle_time >= cpu_cycle_duration) {
            cpu.cycle();
            last_cycle_time = current_time;
        }

        if (current_time - last_timer_time >= timer_duration) {
            cpu.update_timers();
            last_timer_time = current_time;
        }

        cpu.render();

        std::this_thread::sleep_for(std::chrono::microseconds(500));
    }

    return 0;
}