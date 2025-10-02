# CHIP-8 Emulator

A simple CHIP-8 virtual machine implemented in C++17.

## How to Build

```bash
mkdir build
cd build
cmake ..
make
```

## How to Run

Place your CHIP-8 ROM files in the `ROMS` directory.

Then, from the `build` directory, run the emulator with the path to your ROM:

```bash
./chip8 ../ROMS/your_rom_name.ch8
```

For example:

```bash
./chip8 ../ROMS/helloworld.ch8
```

<img width="636" height="314" alt="image" src="https://github.com/user-attachments/assets/5616e7eb-b547-455c-a945-0e1622cfff00" />

Debugger:

<img width="501" height="313" alt="image" src="https://github.com/user-attachments/assets/a82ebd54-34f1-40eb-b938-2ad403f0d344" />
