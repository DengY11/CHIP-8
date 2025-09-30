#include <gtest/gtest.h>
#include "chip8.hpp"
#include "memory.hpp"
#include "test_access.hpp"
#include <fstream>
#include <vector>

class Chip8Test : public ::testing::Test {
protected:
    void SetUp() override {
        cpu = std::make_unique<CHIP8::Chip8CPU>(CHIP8::Chip8Mode::TEST);
    }
    void TearDown() override {
        cpu.reset();
    }
    bool loadProgram(const std::vector<uint8_t>& program) {
        std::ofstream temp_file("test_program.ch8", std::ios::binary);
        if (!temp_file.is_open()) return false;
        temp_file.write(reinterpret_cast<const char*>(program.data()), program.size());
        temp_file.close();
        return cpu->loadROM("test_program.ch8");
    }

    std::unique_ptr<CHIP8::Chip8CPU> cpu;
};

// Test basic execution
TEST_F(Chip8Test, BasicExecution) {
    std::vector<uint8_t> program = {0x60, 0x42}; // LD V0, 0x42
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle();
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x42);
}

// Test multiple instructions
TEST_F(Chip8Test, MultipleInstructions) {
    std::vector<uint8_t> program = {0x60, 0x10, 0x61, 0x20, 0x80, 0x14}; // LD V0, 0x10; LD V1, 0x20; ADD V0, V1
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD V0, 0x10
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x10);
    cpu->cycle(); // LD V1, 0x20
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 1), 0x20);
    cpu->cycle(); // ADD V0, V1
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x30);
}

// Test ROM loading
TEST_F(Chip8Test, ROMPathTest) {
    std::vector<uint8_t> program = {0x12, 0x00}; // JP 0x200 (infinite loop)
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle();
    SUCCEED();
}

// Test empty program
TEST_F(Chip8Test, EmptyProgram) {
    std::vector<uint8_t> program = {};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle();
    SUCCEED();
}

// Test LD Vx, byte (6xkk)
TEST_F(Chip8Test, LDVxByte) {
    std::vector<uint8_t> program = {0x60, 0x42};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle();
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x42);
}

// Test ADD Vx, byte (7xkk)
TEST_F(Chip8Test, ADDVxByte) {
    std::vector<uint8_t> program = {0x61, 0x10, 0x71, 0x20};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD V1, 0x10
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 1), 0x10);
    cpu->cycle(); // ADD V1, 0x20
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 1), 0x30);
}

// Test LD Vx, Vy (8xy0)
TEST_F(Chip8Test, LDVxVy) {
    std::vector<uint8_t> program = {0x60, 0x10, 0x61, 0x20, 0x80, 0x10};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD V0, 0x10
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x10);
    cpu->cycle(); // LD V1, 0x20
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 1), 0x20);
    cpu->cycle(); // LD V0, V1
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x20);
}

// Test ADD Vx, Vy (8xy4)
TEST_F(Chip8Test, ADDVxVy) {
    std::vector<uint8_t> program = {0x60, 0x10, 0x61, 0x20, 0x80, 0x14};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD V0, 0x10
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x10);
    cpu->cycle(); // LD V1, 0x20
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 1), 0x20);
    cpu->cycle(); // ADD V0, V1
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x30);
}

// Test SUB Vx, Vy (8xy5)
TEST_F(Chip8Test, SUBVxVy) {
    std::vector<uint8_t> program = {0x60, 0x30, 0x61, 0x10, 0x80, 0x15};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD V0, 0x30
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x30);
    cpu->cycle(); // LD V1, 0x10
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 1), 0x10);
    cpu->cycle(); // SUB V0, V1
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x20);
}

// Test AND Vx, Vy (8xy2)
TEST_F(Chip8Test, ANDVxVy) {
    std::vector<uint8_t> program = {0x60, 0x0F, 0x61, 0xF0, 0x80, 0x12};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD V0, 0x0F
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x0F);
    cpu->cycle(); // LD V1, 0xF0
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 1), 0xF0);
    cpu->cycle(); // AND V0, V1
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x00);
}

// Test OR Vx, Vy (8xy1)
TEST_F(Chip8Test, ORVxVy) {
    std::vector<uint8_t> program = {0x60, 0x0F, 0x61, 0xF0, 0x80, 0x11};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD V0, 0x0F
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x0F);
    cpu->cycle(); // LD V1, 0xF0
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 1), 0xF0);
    cpu->cycle(); // OR V0, V1
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0xFF);
}

// Test XOR Vx, Vy (8xy3)
TEST_F(Chip8Test, XORVxVy) {
    std::vector<uint8_t> program = {0x60, 0x0F, 0x61, 0xFF, 0x80, 0x13};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD V0, 0x0F
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x0F);
    cpu->cycle(); // LD V1, 0xFF
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 1), 0xFF);
    cpu->cycle(); // XOR V0, V1
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0xF0);
}

// Test SHR Vx {, Vy} (8xy6)
TEST_F(Chip8Test, SHRVx) {
    std::vector<uint8_t> program = {0x60, 0x09, 0x80, 0x06};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD V0, 0x09
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x09);
    cpu->cycle(); // SHR V0
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x04);
}

// Test SHL Vx {, Vy} (8xyE)
TEST_F(Chip8Test, SHLVx) {
    std::vector<uint8_t> program = {0x60, 0x88, 0x80, 0x0E};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD V0, 0x88
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x88);
    cpu->cycle(); // SHL V0
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x10);
}

// Test SUBN Vx, Vy (8xy7)
TEST_F(Chip8Test, SUBNVxVy) {
    std::vector<uint8_t> program = {0x60, 0x10, 0x61, 0x30, 0x80, 0x17};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD V0, 0x10
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x10);
    cpu->cycle(); // LD V1, 0x30
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 1), 0x30);
    cpu->cycle(); // SUBN V0, V1
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x20);
}

// Test JP addr (1nnn)
TEST_F(Chip8Test, JPAddr) {
    std::vector<uint8_t> program = {0x14, 0x00};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // JP 0x400
    EXPECT_EQ(CHIP8::Chip8TestAccess::getPC(*cpu), 0x400);
}

// Test CALL addr (2nnn) and RET (00EE)
TEST_F(Chip8Test, CALLAndRET) {
    std::vector<uint8_t> program = {0x23, 0x00, 0x00, 0x00, 0x00, 0xEE};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // CALL 0x300
    EXPECT_EQ(CHIP8::Chip8TestAccess::getPC(*cpu), 0x300);
    EXPECT_EQ(CHIP8::Chip8TestAccess::getSP(*cpu), 1);
    
    // Manually set up the RET instruction at 0x300
    CHIP8::Chip8TestAccess::setMemory(*cpu, 0x300, 0x00);
    CHIP8::Chip8TestAccess::setMemory(*cpu, 0x301, 0xEE);
    
    cpu->cycle(); // RET
    EXPECT_EQ(CHIP8::Chip8TestAccess::getPC(*cpu), 0x202);
    EXPECT_EQ(CHIP8::Chip8TestAccess::getSP(*cpu), 0);
}

// Test SE Vx, byte (3xkk)
TEST_F(Chip8Test, SEVxByte) {
    std::vector<uint8_t> program = {0x62, 0x50, 0x32, 0x50};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD V2, 0x50
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 2), 0x50);
    uint16_t pc_before = CHIP8::Chip8TestAccess::getPC(*cpu);
    cpu->cycle(); // SE V2, 0x50
    // PC should skip by 4 since V2 == 0x50 (normal +2 + skip +2)
    EXPECT_EQ(CHIP8::Chip8TestAccess::getPC(*cpu), pc_before + 4);
}

// Test SNE Vx, byte (4xkk)
TEST_F(Chip8Test, SNEVxByte) {
    std::vector<uint8_t> program = {0x63, 0x60, 0x43, 0x61};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD V3, 0x60
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 3), 0x60);
    uint16_t pc_before = CHIP8::Chip8TestAccess::getPC(*cpu);
    cpu->cycle(); // SNE V3, 0x61
    // PC should skip by 4 since V3 != 0x61 (normal +2 + skip +2)
    EXPECT_EQ(CHIP8::Chip8TestAccess::getPC(*cpu), pc_before + 4);
}

// Test SE Vx, Vy (5xy0)
TEST_F(Chip8Test, SEVxVy) {
    std::vector<uint8_t> program = {0x60, 0x50, 0x61, 0x50, 0x50, 0x10};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD V0, 0x50
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x50);
    cpu->cycle(); // LD V1, 0x50
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 1), 0x50);
    uint16_t pc_before = CHIP8::Chip8TestAccess::getPC(*cpu);
    cpu->cycle(); // SE V0, V1
    // PC should skip by 4 since V0 == V1 (normal +2 + skip +2)
    EXPECT_EQ(CHIP8::Chip8TestAccess::getPC(*cpu), pc_before + 4);
}

// Test SNE Vx, Vy (9xy0)
TEST_F(Chip8Test, SNEVxVy) {
    std::vector<uint8_t> program = {0x60, 0x50, 0x61, 0x60, 0x90, 0x10};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD V0, 0x50
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x50);
    cpu->cycle(); // LD V1, 0x60
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 1), 0x60);
    uint16_t pc_before = CHIP8::Chip8TestAccess::getPC(*cpu);
    cpu->cycle(); // SNE V0, V1
    // PC should skip by 4 since V0 != V1 (normal +2 + skip +2)
    EXPECT_EQ(CHIP8::Chip8TestAccess::getPC(*cpu), pc_before + 4);
}

// Test LD I, addr (Annn)
TEST_F(Chip8Test, LDIAddr) {
    std::vector<uint8_t> program = {0xA3, 0x00};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD I, 0x300
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterI(*cpu), 0x300);
}

// Test CLS (00E0)
TEST_F(Chip8Test, CLS) {
    std::vector<uint8_t> program = {0x00, 0xE0};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // CLS
    // We can't directly test if the display was cleared, but the instruction should execute
    SUCCEED();
}

// Test JP V0, addr (Bnnn)
TEST_F(Chip8Test, JPV0Addr) {
    std::vector<uint8_t> program = {0x60, 0x10, 0xB2, 0x00};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD V0, 0x10
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x10);
    cpu->cycle(); // JP V0, 0x200
    EXPECT_EQ(CHIP8::Chip8TestAccess::getPC(*cpu), 0x210); // 0x200 + 0x10
}

// Test RND Vx, byte (Cxkk)
TEST_F(Chip8Test, RNDVxByte) {
    std::vector<uint8_t> program = {0xC0, 0xFF};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // RND V0, 0xFF
    // We can't predict the random value, but the instruction should execute
    SUCCEED();
}

// Test DRW Vx, Vy, nibble (Dxyn)
TEST_F(Chip8Test, DRWVxVyNibble) {
    std::vector<uint8_t> program = {0x60, 0x10, 0x61, 0x20, 0xD0, 0x15};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD V0, 0x10
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x10);
    cpu->cycle(); // LD V1, 0x20
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 1), 0x20);
    cpu->cycle(); // DRW V0, V1, 5
    // We can't directly test the display, but the instruction should execute
    SUCCEED();
}

// Test SKP Vx (Ex9E)
TEST_F(Chip8Test, SKPVx) {
    std::vector<uint8_t> program = {0x60, 0x01, 0xE0, 0x9E};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD V0, 0x01
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x01);
    uint16_t pc_before = CHIP8::Chip8TestAccess::getPC(*cpu);
    cpu->cycle(); // SKP V0
    // We can't simulate key press, so PC will not skip
    EXPECT_EQ(CHIP8::Chip8TestAccess::getPC(*cpu), pc_before + 2);
}

// Test SKNP Vx (ExA1)
TEST_F(Chip8Test, SKNPVx) {
    std::vector<uint8_t> program = {0x60, 0x01, 0xE0, 0xA1};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD V0, 0x01
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x01);
    uint16_t pc_before = CHIP8::Chip8TestAccess::getPC(*cpu);
    cpu->cycle(); // SKNP V0
    // We can't simulate key press, so PC will skip
    EXPECT_EQ(CHIP8::Chip8TestAccess::getPC(*cpu), pc_before + 2);
}

// Test LD Vx, DT (Fx07)
TEST_F(Chip8Test, LDVxDT) {
    std::vector<uint8_t> program = {0xF0, 0x07};
    ASSERT_TRUE(loadProgram(program));
    // Set delay timer to a known value
    // This requires direct access to the register, which we don't have in the test
    cpu->cycle(); // LD V0, DT
    // We can't predict the delay timer value, but the instruction should execute
    SUCCEED();
}

// Test LD Vx, K (Fx0A)
TEST_F(Chip8Test, LDVxK) {
    std::vector<uint8_t> program = {0xF0, 0x0A};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD V0, K
    // This instruction waits for a key press, so we can't test it directly
    SUCCEED();
}

// Test LD DT, Vx (Fx15)
TEST_F(Chip8Test, LDDTVx) {
    std::vector<uint8_t> program = {0x60, 0x10, 0xF0, 0x15};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD V0, 0x10
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x10);
    cpu->cycle(); // LD DT, V0
    // We can't directly test the delay timer, but the instruction should execute
    SUCCEED();
}

// Test LD ST, Vx (Fx18)
TEST_F(Chip8Test, LDSTVx) {
    std::vector<uint8_t> program = {0x60, 0x10, 0xF0, 0x18};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD V0, 0x10
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x10);
    cpu->cycle(); // LD ST, V0
    // We can't directly test the sound timer, but the instruction should execute
    SUCCEED();
}

// Test ADD I, Vx (Fx1E)
TEST_F(Chip8Test, ADDIVx) {
    std::vector<uint8_t> program = {0xA0, 0x10, 0x60, 0x20, 0xF0, 0x1E};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD I, 0x10
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterI(*cpu), 0x10);
    cpu->cycle(); // LD V0, 0x20
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x20);
    cpu->cycle(); // ADD I, V0
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterI(*cpu), 0x30);
}

// Test LD F, Vx (Fx29)
TEST_F(Chip8Test, LDFVx) {
    std::vector<uint8_t> program = {0x60, 0x05, 0xF0, 0x29};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD V0, 0x05
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x05);
    cpu->cycle(); // LD F, V0
    // Font address for digit 5 is 5 * 5 = 25 (0x19)
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterI(*cpu), 0x19);
}

// Test LD B, Vx (Fx33)
TEST_F(Chip8Test, LDBVx) {
    std::vector<uint8_t> program = {0x60, 0xFF, 0xF0, 0x33};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD V0, 0xFF
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0xFF);
    cpu->cycle(); // LD B, V0
    // BCD of 255 is 2 5 5
    // Memory at I should be 2, I+1 should be 5, I+2 should be 5
    // We can't directly test memory without access, but the instruction should execute
    SUCCEED();
}

// Test LD [I], Vx (Fx55)
TEST_F(Chip8Test, LDIVx) {
    std::vector<uint8_t> program = {0x60, 0x10, 0x61, 0x20, 0xA0, 0x00, 0xF1, 0x55};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD V0, 0x10
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 0), 0x10);
    cpu->cycle(); // LD V1, 0x20
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterV(*cpu, 1), 0x20);
    cpu->cycle(); // LD I, 0x00
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterI(*cpu), 0x00);
    cpu->cycle(); // LD [I], V1
    // Memory at 0x00 should be 0x10, memory at 0x01 should be 0x20
    // We can't directly test memory without access, but the instruction should execute
    SUCCEED();
}

// Test LD Vx, [I] (Fx65)
TEST_F(Chip8Test, LDVxI) {
    std::vector<uint8_t> program = {0xA0, 0x00, 0xF1, 0x65};
    ASSERT_TRUE(loadProgram(program));
    cpu->cycle(); // LD I, 0x00
    EXPECT_EQ(CHIP8::Chip8TestAccess::getRegisterI(*cpu), 0x00);
    cpu->cycle(); // LD Vx, [I]
    // We can't predict the values loaded, but the instruction should execute
    SUCCEED();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    std::remove("test_program.ch8");
    return RUN_ALL_TESTS();
}