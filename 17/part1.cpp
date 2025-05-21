#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>

#include "../scanner.h"

enum Opcode
{
    ADV = 0, // division of A / 2^(combo operand) -> A
    BXL = 1, // bitwise XOR of B and literal operand -> B
    BST = 2, // modulo 8 of combo operand -> B
    JNZ = 3, // jump by literal operand if A != 0
    BXC = 4, // bitwise XOR of B and C -> B
    OUT = 5, // outputs combo operand
    BDV = 6, // adv -> B
    CDV = 7, // adv -> C
};

class Interpreter
{
private:
    int ip;

    int registers[3]; // [A, B, C]
    const std::vector<uint8_t> opcodes;

    bool finished()
    {
        return ip >= opcodes.size();
    }

    int calculateComboOperand(uint8_t operand);

    // 'pop' a value off the opcodes vector.
    // Popping only increases the ip by 1, and doesn't remove anything
    // from the vector.
    // Values should never be removed, as looping may require them again.
    uint8_t pop()
    {
        return opcodes[ip++];
    }

    Opcode popOpcode()
    {
        return (Opcode)pop();
    }

public:
    Interpreter(const std::vector<uint8_t> &codes, int a, int b, int c) : opcodes(codes)
    {
        registers[0] = a;
        registers[1] = b;
        registers[2] = c;

        ip = 0;
    }

    // Run the interpreter, starting from ip = 0 and executing the program until ip >= opcodes.size();
    void run();
};

int Interpreter::calculateComboOperand(uint8_t operand)
{
    // 0 - 3 == literal
    if (operand < 0)
        return -1;
    switch (operand)
    {
    case 0:
    case 1:
    case 2:
    case 3:
        return (int)operand;
    case 4:
        // register A
        return registers[0];
    case 5:
        // register B
        return registers[1];
    case 6:
        // register C
        return registers[2];
    case 7:
    default:
        // reserved
        std::cout << "\t RESERVED OPCODE\n";
        return -1;
    }
}

void Interpreter::run()
{
    while (!finished())
    {
        Opcode oper = popOpcode();

        switch (oper)
        {
        case ADV:
            printf("opcode <adv>\n");
            break;
        case BXL:
        case BST:
        case JNZ:
        case BXC:
        case OUT:
        {
            uint8_t operand = pop();
            int combo = calculateComboOperand(operand) % 8;
            printf("<out> %d\n", combo);
            break;
        }
        case BDV:
        case CDV:
        default:
            printf("Unknown opcode <%d>.\n", oper);
            break;
        }
    }

    printf("Finished execution.\n");
}

int main()
{
    std::string input("input.txt");
    std::ifstream file(input);
    ScanSequence ss(file);
    file.close();

    // // Scan initial values
    // ss.consumeString("Register A: ");
    // int reg_a = ss.scanInt();
    // ss.consumeString("\nRegister B: ");
    // int reg_b = ss.scanInt();
    // ss.consumeString("\nRegister C: ");
    // int reg_c = ss.scanInt();

    // ss.consumeString("\n\nProgram: ");
    // std::vector<uint8_t> opcodes;
    // ss.applyWithDelim(',', [&opcodes](ScanSequence &ss)
    //                   { opcodes.push_back(ss.scanUInt8()); });

    // // printf("Register A: %d\nRegister B: %d\nRegister C: %d\n", reg_a, reg_b, reg_c);
    // // std::cout << "Program: ";
    // // for (auto x : opcodes)
    // // {
    // //     std::cout << (int)x << ",";
    // // }
    // // std::cout << "\n";

    // // Start the actual program
    // Interpreter interpreter(opcodes, reg_a, reg_b, reg_c);
    // interpreter.run();

    // testing
    Interpreter interpreter({5, 3}, 1, 0, 0);
    interpreter.run();
}