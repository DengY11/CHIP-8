#include "chip8.hpp"
#include "memory.hpp"
#include "register.hpp"
#include "stack.hpp"
#include "display.hpp"
#include <iomanip> 
#include "input.hpp"
#include <iostream>
#include <random>
#include <thread>

// Fontset for CHIP-8
static const uint8_t fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
    0x20, 0x60, 0x20, 0x20, 0x70,  // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
    0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
    0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
    0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
    0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
    0xF0, 0x80, 0xF0, 0x80, 0x80   // F
};

namespace CHIP8 {

Chip8CPU::Chip8CPU() : Chip8CPU(Chip8Mode::NORMAL) {}

Chip8CPU::Chip8CPU(Chip8Mode mode) {
    mem = std::make_unique<Memory>();
    reg = std::make_unique<Register>();
    
    stack = std::make_unique<Stack>(reg->SP, stack_array);
    
    if (mode == Chip8Mode::NORMAL) {
        display = std::make_unique<Chip8Display>();
        keypad = std::make_unique<Chip8Keypad>();
    } else {
        display = nullptr;
        keypad = nullptr;
    }
    mem->loadFontset(fontset, sizeof(fontset));
}

Chip8CPU::Chip8CPU(Chip8Mode mode,const std::string& rom_path) : Chip8CPU(mode) {
    if (!loadROM(rom_path)) {
        throw std::runtime_error("Failed to load ROM: " + rom_path);
    }
}

void Chip8CPU::run() {
    using clock = std::chrono::high_resolution_clock;
    auto last_cycle_time = clock::now();
    auto last_timer_time = clock::now();
    const std::chrono::duration<double> cpu_cycle_duration(1.0 /
                                                           500.0);   // 500 Hz
    const std::chrono::duration<double> timer_duration(1.0 / 60.0);  // 60 Hz
    while (handle_input()) {
        auto current_time = clock::now();
        if (current_time - last_cycle_time >= cpu_cycle_duration) {
           cycle();
            last_cycle_time = current_time;
        }
        if (current_time - last_timer_time >= timer_duration) {
           update_timers();
            last_timer_time = current_time;
        }
       render();
        std::this_thread::sleep_for(std::chrono::microseconds(500));
    }
}

void Chip8CPU::cycle() {
    std::cout << "PC: 0x" << std::hex << std::setw(4) << std::setfill('0')
              << reg->PC << ", Opcode: 0x" << std::hex << std::setw(4)
              << std::setfill('0')
              << ((mem->readByte(reg->PC).value_or(0) << 8) |
                  mem->readByte(reg->PC + 1).value_or(0))
              << std::endl;

    uint8_t high_byte = this->mem->readByte(this->reg->PC).value_or(0);
    uint8_t low_byte = this->mem->readByte(this->reg->PC + 1).value_or(0);
    uint16_t opcode = (high_byte << 8) | low_byte;

    auto& V = reg->V;
    auto& I = reg->I;
    auto& PC = reg->PC;
    auto& SP = reg->SP;

    uint16_t nnn = opcode & 0x0FFF;
    uint8_t nn = opcode & 0x00FF;
    uint8_t n = opcode & 0x000F;
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    switch (opcode & 0xF000) {
        case 0x0000:
            switch (nn) {
                case 0xE0:  // 00E0: CLS
                    if (display) {
                        display->clear();
                    }
                    break;
                case 0xEE:                          // 00EE: RET
                    PC = stack->pop().value_or(0);  // Safely unwrap optional
                    break;
            }
            break;
        case 0x1000:  // 1NNN: JP addr
            PC = nnn;
            return;   // Return to avoid PC += 2
        case 0x2000:  // 2NNN: CALL addr
            stack->push(PC);
            PC = nnn;
            return;   // Return to avoid PC += 2
        case 0x3000:  // 3XNN: SE Vx, byte
            if (V[x] == nn) PC += 2;
            break;
        case 0x4000:  // 4XNN: SNE Vx, byte
            if (V[x] != nn) PC += 2;
            break;
        case 0x5000:  // 5XY0: SE Vx, Vy
            if (V[x] == V[y]) PC += 2;
            break;
        case 0x6000:  // 6XNN: LD Vx, byte
            V[x] = nn;
            break;
        case 0x7000:  // 7XNN: ADD Vx, byte
            V[x] += nn;
            break;
        case 0x8000:
            switch (n) {
                case 0x0:
                    V[x] = V[y];
                    break;  // 8XY0: LD Vx, Vy
                case 0x1:
                    V[x] |= V[y];
                    break;  // 8XY1: OR Vx, Vy
                case 0x2:
                    V[x] &= V[y];
                    break;  // 8XY2: AND Vx, Vy
                case 0x3:
                    V[x] ^= V[y];
                    break;   // 8XY3: XOR Vx, Vy
                case 0x4: {  // 8XY4: ADD Vx, Vy
                    uint16_t sum = V[x] + V[y];
                    V[0xF] = sum > 255;
                    V[x] = sum & 0xFF;
                    break;
                }
                case 0x5: {  // 8XY5: SUB Vx, Vy
                    V[0xF] = V[x] > V[y];
                    V[x] -= V[y];
                    break;
                }
                case 0x6: {  // 8XY6: SHR Vx
                    V[0xF] = V[x] & 0x1;
                    V[x] >>= 1;
                    break;
                }
                case 0x7: {  // 8XY7: SUBN Vx, Vy
                    V[0xF] = V[y] > V[x];
                    V[x] = V[y] - V[x];
                    break;
                }
                case 0xE: {  // 8XYE: SHL Vx
                    V[0xF] = (V[x] & 0x80) >> 7;
                    V[x] <<= 1;
                    break;
                }
            }
            break;
        case 0x9000:  // 9XY0: SNE Vx, Vy
            if (V[x] != V[y]) PC += 2;
            break;
        case 0xA000:  // ANNN: LD I, addr
            I = nnn;
            break;
        case 0xB000:  // BNNN: JP V0, addr
            PC = nnn + V[0];
            return;     // Return to avoid PC += 2
        case 0xC000: {  // CXNN: RND Vx, byte
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(0, 255);
            V[x] = distrib(gen) & nn;
            break;
        }
        case 0xD000: {  // DXYN: DRW Vx, Vy, nibble
            if (display) {
                const uint8_t* sprite = mem->getRawMemory() + I;
                V[0xF] = display->drawSprite(V[x], V[y], sprite, n);
            }
            break;
        }
        case 0xE000:
            switch (nn) {
                case 0x9E:  // EX9E: SKP Vx
                    if (keypad && keypad->isKeyPressed(V[x])) PC += 2;
                    break;
                case 0xA1:  // EXA1: SKNP Vx
                    if (keypad && !keypad->isKeyPressed(V[x])) PC += 2;
                    break;
            }
            break;
        case 0xF000:
            switch (nn) {
                case 0x07:
                    V[x] = reg->delay_timer;
                    break;  // FX07: LD Vx, DT
                case 0x0A:  // FX0A: LD Vx, K
                    if (keypad) {
                        V[x] = keypad->waitForKey();
                    } else {
                        // In test mode, return a default value (e.g., 0)
                        V[x] = 0;
                    }
                    break;
                case 0x15:
                    reg->delay_timer = V[x];
                    break;  // FX15: LD DT, Vx
                case 0x18:
                    reg->sound_timer = V[x];
                    break;  // FX18: LD ST, Vx
                case 0x1E:
                    I += V[x];
                    break;  // FX1E: ADD I, Vx
                case 0x29:
                    I = V[x] * 5;
                    break;    // FX29: LD F, Vx (Fontset is at 0x0)
                case 0x33: {  // FX33: LD B, Vx
                    uint8_t val = V[x];
                    mem->writeByte(I + 2, val % 10);
                    val /= 10;
                    mem->writeByte(I + 1, val % 10);
                    val /= 10;
                    mem->writeByte(I, val % 10);
                    break;
                }
                case 0x55: {  // FX55: LD [I], Vx
                    for (int i = 0; i <= x; ++i) {
                        mem->writeByte(I + i, V[i]);
                    }
                    break;
                }
                case 0x65: {  // FX65: LD Vx, [I]
                    for (int i = 0; i <= x; ++i) {
                        V[i] = mem->readByte(I + i).value_or(0);
                    }
                    break;
                }
            }
            break;
        default:
            // Unknown opcode
            break;
    }

    PC += 2;
}

bool Chip8CPU::loadROM(const std::string& filename) {
    return mem->loadROM(filename);
}

void Chip8CPU::update_timers() {
    if (reg->delay_timer > 0) {
        reg->delay_timer--;
    }
    if (reg->sound_timer > 0) {
        reg->sound_timer--;
        // TODO: Add sound output logic here
    }
}

bool Chip8CPU::handle_input() {
    if (keypad) {
        return keypad->handleInput();
    }
    return false;
}

void Chip8CPU::render() {
    if (display) {
        display->render();
    }
}

}  // namespace CHIP8
