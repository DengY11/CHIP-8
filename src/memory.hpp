#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>

namespace CHIP8 {

static constexpr uint16_t ROM_START_ADDR = 0x200;

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
    bool loadROM(const std::string& filename);

    const uint8_t* getRawMemory() const;
    uint8_t* getRawMemory();

    static bool isLegalAddr(uint16_t address);

private:
    static const size_t MEM_SIZE = 4096;
    ::std::unique_ptr<uint8_t[]> mem;
};

}  // namespace CHIP8
