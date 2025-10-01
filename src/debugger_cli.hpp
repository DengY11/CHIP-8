#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "debugger.hpp"

namespace CHIP8 {

class DebuggerCLI {
public:
    DebuggerCLI(Debugger& debugger);
    ~DebuggerCLI() = default;
    
    void run();
    
private:
    Debugger& debugger;
    
    void printHelp();
    void printPrompt();
    bool parseCommand(const std::string& input);
    
    // 命令处理函数
    void handleStep(const std::vector<std::string>& args);
    void handleContinue(const std::vector<std::string>& args);
    void handleBreakpoint(const std::vector<std::string>& args);
    void handleDeleteBreakpoint(const std::vector<std::string>& args);
    void handleClearBreakpoints(const std::vector<std::string>& args);
    void handleRegisters(const std::vector<std::string>& args);
    void handleMemory(const std::vector<std::string>& args);
    void handleQuit(const std::vector<std::string>& args);
    void handleHelp(const std::vector<std::string>& args);
};

}  // namespace CHIP8