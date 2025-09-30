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

void Chip8TestAccess::setMemory(Chip8CPU& cpu, uint16_t address, uint8_t value) { 
    cpu.mem->writeByte(address, value); 
}

// Test access methods for Memory
uint8_t Chip8TestAccess::getMemory(const Memory& mem, uint16_t address) { 
    return mem.readByte(address).value_or(0); 
}

void Chip8TestAccess::setMemory(Memory& mem, uint16_t address, uint8_t value) { 
    mem.writeByte(address, value); 
}

}  // namespace CHIP8