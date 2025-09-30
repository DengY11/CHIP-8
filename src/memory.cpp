#include "memory.hpp"

#include <cstring>
#include <fstream>
#include <iterator>
#include <vector>

namespace CHIP8 {

static constexpr uint16_t ROM_START_ADDR = 0x200;

Memory::Memory() {
    this->mem = std::make_unique<uint8_t[]>(MEM_SIZE);
    memset(mem.get(), 0, MEM_SIZE);
}

void Memory::reset() {
    this->mem = std::make_unique<uint8_t[]>(MEM_SIZE);
    memset(mem.get(), 0, MEM_SIZE);
}

bool Memory::isLegalAddr(uint16_t address) {
    return address < MEM_SIZE;
}

std::optional<uint8_t> Memory::readByte(uint16_t address) const {
    if (isLegalAddr(address)) {
        return mem[address];
    }
    return std::nullopt;
}

bool Memory::writeByte(uint16_t address, uint8_t value) {
    if (isLegalAddr(address)) {
        mem[address] = value;
        return true;
    }
    return false;
}

std::optional<uint16_t> Memory::readWord(uint16_t addr) const {
    if (isLegalAddr(addr) && isLegalAddr(addr + 1)) {
        uint16_t ret = 0;
        ret |= (static_cast<uint16_t>(mem[addr])) << 8;
        ret |= static_cast<uint16_t>(mem[addr + 1]);
        return ret;
    }
    return std::nullopt;
}

bool Memory::writeWord(uint16_t addr, uint16_t value) {
    if (isLegalAddr(addr) && isLegalAddr(addr + 1)) {
        mem[addr] = (value & 0xFF00) >> 8;
        mem[addr + 1] = value & 0x00FF;
        return true;
    }
    return false;
}

void Memory::loadFontset(const uint8_t* fontset, size_t size) {
    if (mem) {
        std::memcpy(mem.get(), fontset, size);
    }
}

bool Memory::loadROM(const std::string& filename) {
    if (!mem) {
        mem = std::make_unique<uint8_t[]>(MEM_SIZE);
    }
    std::ifstream rom_file(filename, std::ios::binary | std::ios::ate);
    if (!rom_file.is_open()) {
        return false;
    }
    std::streamsize size = rom_file.tellg();
    rom_file.seekg(0, std::ios::beg);
    if (size > MEM_SIZE - ROM_START_ADDR) {
        return false;
    }
    std::vector<char> buffer(size);
    if (rom_file.read(buffer.data(), size)) {
        std::copy(buffer.begin(), buffer.end(), &mem[ROM_START_ADDR]);
        return true;
    }
    return false;
}

void Memory::loadROM(const std::vector<uint8_t>& data) {
    if (!mem) {
        mem = std::make_unique<uint8_t[]>(MEM_SIZE);
    }
    if (data.size() > MEM_SIZE - ROM_START_ADDR) {
        return;
    }
    std::copy(data.begin(), data.end(), &mem[ROM_START_ADDR]);
}

const uint8_t* Memory::getRawMemory() const {
    return mem.get();
}

uint8_t* Memory::getRawMemory() {
    return mem.get();
}

}  // namespace CHIP8
