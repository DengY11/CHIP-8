#include "test_access.hpp"

#include "chip8.hpp"
#include "memory.hpp"

namespace CHIP8 {

// Test access methods for Chip8CPU
uint8_t Chip8TestAccess::getRegisterV(const Chip8CPU& cpu, int index) {
    return cpu.reg->V[index];
}

uint16_t Chip8TestAccess::getRegisterI(const Chip8CPU& cpu) {
    return cpu.reg->I;
}

uint16_t Chip8TestAccess::getPC(const Chip8CPU& cpu) {
    return cpu.reg->PC;
}

uint8_t Chip8TestAccess::getSP(const Chip8CPU& cpu) {
    return cpu.reg->SP;
}

uint8_t Chip8TestAccess::getDT(const Chip8CPU& cpu) {
    return cpu.reg->delay_timer;
}

uint8_t Chip8TestAccess::getST(const Chip8CPU& cpu) {
    return cpu.reg->sound_timer;
}

uint8_t Chip8TestAccess::getMemory(const Chip8CPU& cpu, uint16_t address) {
    return cpu.mem->readByte(address).value_or(0);
}

void Chip8TestAccess::setMemory(Chip8CPU& cpu, uint16_t address,
                                uint8_t value) {
    cpu.mem->writeByte(address, value);
}

void Chip8TestAccess::cycle(Chip8CPU& cpu) {
    cpu.cycle();
}

bool Chip8TestAccess::loadROM(Chip8CPU& cpu, const std::string& filename) {
    return cpu.loadROM(filename);
}

void Chip8TestAccess::update_timers(Chip8CPU& cpu) {
    cpu.update_timers();
}

bool Chip8TestAccess::handle_input(Chip8CPU& cpu) {
    return cpu.handle_input();
}

void Chip8TestAccess::render(Chip8CPU& cpu) {
    cpu.render();
}

// Setter methods for Chip8CPU
void Chip8TestAccess::setRegisterV(Chip8CPU& cpu, int index, uint8_t value) {
    cpu.reg->V[index] = value;
}

void Chip8TestAccess::setRegisterI(Chip8CPU& cpu, uint16_t value) {
    cpu.reg->I = value;
}

void Chip8TestAccess::setPC(Chip8CPU& cpu, uint16_t value) {
    cpu.reg->PC = value;
}

void Chip8TestAccess::setSP(Chip8CPU& cpu, uint8_t value) {
    cpu.reg->SP = value;
}

void Chip8TestAccess::setDT(Chip8CPU& cpu, uint8_t value) {
    cpu.reg->delay_timer = value;
}

void Chip8TestAccess::setST(Chip8CPU& cpu, uint8_t value) {
    cpu.reg->sound_timer = value;
}

}  // namespace CHIP8