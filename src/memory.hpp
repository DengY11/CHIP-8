#pragma once
#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "test_access.hpp"

namespace CHIP8 {
class Memory {
public:
    Memory();
    ~Memory() = default;
    void reset();
    std::optional<uint8_t> readByte(uint16_t address) const;
    bool writeByte(uint16_t address, uint8_t value);
    std::optional<uint16_t> readWord(uint16_t address) const;
    bool writeWord(uint16_t address, uint16_t value);
    void loadFontset(const uint8_t* fontset, size_t size);
    const uint8_t* getRawMemory() const;
    uint8_t* getRawMemory();
    static bool isLegalAddr(uint16_t address);

    bool loadROM(const std::string& filename);
    void loadROM(const std::vector<uint8_t>& data);

private:
    static const size_t MEM_SIZE = 4096;
    ::std::unique_ptr<uint8_t[]> mem;

    // Friend class for testing
    friend class Chip8TestAccess;
};
}  // namespace CHIP8
