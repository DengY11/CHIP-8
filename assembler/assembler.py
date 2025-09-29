import sys
import re

def main():
    if len(sys.argv) != 3:
        print("Usage: python assembler.py <input_file.asm> <output_file.ch8>")
        sys.exit(1)

    input_path = sys.argv[1]
    output_path = sys.argv[2]

    with open(input_path, 'r') as f:
        source_lines = f.readlines()

    # --- First Pass: Build labels table ---
    labels = {}
    instructions = []
    current_address = 0x200

    for line in source_lines:
        line = re.sub(r';.*$', '', line).strip()
        if not line:
            continue

        match = re.match(r'^(\w+):$', line, re.IGNORECASE)
        if match:
            labels[match.group(1).upper()] = current_address
        else:
            instructions.append(line)
            current_address += 2

    # --- Second Pass: Assemble instructions ---
    rom_data = bytearray()
    for instruction in instructions:
        # Improved instruction parsing
        parts = instruction.strip().split(maxsplit=1)
        mnemonic = parts[0].upper()
        args_str = parts[1] if len(parts) > 1 else ''
        args = [arg.strip() for arg in re.split(r'\s*,\s*', args_str)] if args_str else []

        opcode = 0x0000

        try:
            if mnemonic == 'CLS':
                opcode = 0x00E0
            elif mnemonic == 'RET':
                opcode = 0x00EE
            elif mnemonic == 'JP':
                if args[0].upper() == 'V0':
                    addr = get_value(args[1], labels)
                    opcode = 0xB000 | addr
                else:
                    addr = get_value(args[0], labels)
                    opcode = 0x1000 | addr
            elif mnemonic == 'CALL':
                addr = get_value(args[0], labels)
                opcode = 0x2000 | addr
            elif mnemonic == 'SE':
                vx = get_value(args[0], labels)
                if is_register(args[1]):
                    vy = get_value(args[1], labels)
                    opcode = 0x5000 | (vx << 8) | (vy << 4)
                else:
                    byte = get_value(args[1], labels)
                    opcode = 0x3000 | (vx << 8) | byte
            elif mnemonic == 'SNE':
                vx = get_value(args[0], labels)
                if is_register(args[1]):
                    vy = get_value(args[1], labels)
                    opcode = 0x9000 | (vx << 8) | (vy << 4)
                else:
                    byte = get_value(args[1], labels)
                    opcode = 0x4000 | (vx << 8) | byte
            elif mnemonic == 'LD':
                arg1, arg2 = args[0].upper(), args[1].upper()
                if is_register(arg1):
                    vx = get_value(arg1, labels)
                    if is_register(arg2):
                        vy = get_value(arg2, labels)
                        opcode = 0x8000 | (vx << 8) | (vy << 4)
                    elif arg2 == 'DT':
                        opcode = 0xF007 | (vx << 8)
                    elif arg2 == 'K':
                        opcode = 0xF00A | (vx << 8)
                    elif arg2 == '[I]':
                        opcode = 0xF065 | (vx << 8)
                    else:
                        byte = get_value(args[1], labels)
                        opcode = 0x6000 | (vx << 8) | byte
                elif arg1 == 'I':
                    addr = get_value(args[1], labels)
                    opcode = 0xA000 | addr
                elif arg1 == 'DT':
                    vx = get_value(args[1], labels)
                    opcode = 0xF015 | (vx << 8)
                elif arg1 == 'ST':
                    vx = get_value(args[1], labels)
                    opcode = 0xF018 | (vx << 8)
                elif arg1 == 'F':
                    vx = get_value(args[1], labels)
                    opcode = 0xF029 | (vx << 8)
                elif arg1 == 'B':
                    vx = get_value(args[1], labels)
                    opcode = 0xF033 | (vx << 8)
                elif arg1 == '[I]':
                    vx = get_value(args[1], labels)
                    opcode = 0xF055 | (vx << 8)
            elif mnemonic == 'ADD':
                vx = get_value(args[0], labels)
                if args[0].upper() == 'I':
                    opcode = 0xF01E | (vx << 8)
                elif is_register(args[1]):
                    vy = get_value(args[1], labels)
                    opcode = 0x8004 | (vx << 8) | (vy << 4)
                else:
                    byte = get_value(args[1], labels)
                    opcode = 0x7000 | (vx << 8) | byte
            elif mnemonic == 'OR':
                vx, vy = get_value(args[0], labels), get_value(args[1], labels)
                opcode = 0x8001 | (vx << 8) | (vy << 4)
            elif mnemonic == 'AND':
                vx, vy = get_value(args[0], labels), get_value(args[1], labels)
                opcode = 0x8002 | (vx << 8) | (vy << 4)
            elif mnemonic == 'XOR':
                vx, vy = get_value(args[0], labels), get_value(args[1], labels)
                opcode = 0x8003 | (vx << 8) | (vy << 4)
            elif mnemonic == 'SUB':
                vx, vy = get_value(args[0], labels), get_value(args[1], labels)
                opcode = 0x8005 | (vx << 8) | (vy << 4)
            elif mnemonic == 'SHR':
                vx = get_value(args[0], labels)
                opcode = 0x8006 | (vx << 8)
            elif mnemonic == 'SUBN':
                vx, vy = get_value(args[0], labels), get_value(args[1], labels)
                opcode = 0x8007 | (vx << 8) | (vy << 4)
            elif mnemonic == 'SHL':
                vx = get_value(args[0], labels)
                opcode = 0x800E | (vx << 8)
            elif mnemonic == 'RND':
                vx, byte = get_value(args[0], labels), get_value(args[1], labels)
                opcode = 0xC000 | (vx << 8) | byte
            elif mnemonic == 'DRW':
                vx, vy, n = get_value(args[0], labels), get_value(args[1], labels), get_value(args[2], labels)
                opcode = 0xD000 | (vx << 8) | (vy << 4) | n
            elif mnemonic == 'SKP':
                vx = get_value(args[0], labels)
                opcode = 0xE09E | (vx << 8)
            elif mnemonic == 'SKNP':
                vx = get_value(args[0], labels)
                opcode = 0xE0A1 | (vx << 8)
            else:
                raise ValueError(f"Unknown mnemonic: {mnemonic}")

            rom_data.extend(opcode.to_bytes(2, 'big'))
        except Exception as e:
            print(f"Error assembling line: '{instruction}'")
            print(e)
            sys.exit(1)

    with open(output_path, 'wb') as f:
        f.write(rom_data)

    print(f"Successfully assembled '{input_path}' to '{output_path}'")

def is_register(arg):
    return re.match(r'^V[0-9A-F]$', arg, re.IGNORECASE)

def get_value(arg, labels):
    arg_upper = arg.upper()
    if is_register(arg_upper):
        return int(arg_upper[1], 16)
    if arg_upper in labels:
        return labels[arg_upper]
    if arg_upper.startswith('0X'):
        return int(arg_upper, 16)
    return int(arg_upper)

if __name__ == '__main__':
    main()