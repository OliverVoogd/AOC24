#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <sstream>
#include <stdint.h>
#include <numeric>

#include "../scanner.h"

// #define DEBUG_PRINT

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

const char *OpcodeNames[] = {
    "adv", "bxl", "bst", "jnz", "bxc", "out", "bdv", "cdv"};

typedef struct
{
    int A;
    int B;
    int C;
} Registers;

class Interpreter
{
private:
    int ip;

    Registers registers; // [A, B, C]
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

    uint8_t peek()
    {
        return opcodes[ip];
    }

    Opcode popOpcode()
    {
        return (Opcode)pop();
    }

public:
    Interpreter(const std::vector<uint8_t> &codes, int a, int b, int c) : opcodes(codes)
    {
        registers.A = a;
        registers.B = b;
        registers.C = c;

        ip = 0;
    }

    // Run the interpreter, starting from ip = 0 and executing the program until ip >= opcodes.size();
    std::vector<int> run();
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
        return registers.A;
    case 5:
        // register B
        return registers.B;
    case 6:
        // register C
        return registers.C;
    case 7:
    default:
        // reserved
        std::cout << "\t RESERVED OPCODE\n";
        return -1;
    }
}

static int calculateDivision(int numerator, int denominator, int base = 2)
{
    return numerator / std::pow(2, denominator);
}

std::vector<int> Interpreter::run()
{
    std::vector<int> outputs;
    while (!finished())
    {
        Opcode oper = popOpcode();
#ifdef DEBUG_PRINT
        printf("\t<registers> A=%d, B=%d, C=%d\n", registers.A, registers.B, registers.C);
        printf("<opcode> %s[%d] <operand?> %d\n", OpcodeNames[oper], oper, peek());

#endif
        switch (oper)
        {
        case ADV:
        {
            int div = calculateDivision(registers.A, calculateComboOperand(pop()));
            registers.A = div;
            break;
        }
        case BXL:
            registers.B = registers.B ^ pop();
            break;
        case BST:
        {
            uint8_t val = pop();
            int combo = calculateComboOperand(val);
#ifdef DEBUG_PRINT
            printf("\t\t%d\n", combo);
#endif
            registers.B = combo % 8;
            break;
        }
        case JNZ:
        {
            uint8_t jump_ip = pop();
            if (registers.A != 0)
            {
                ip = jump_ip;
            }
            break;
        }
        case BXC:
            registers.B = registers.B ^ registers.C;
            pop();
            break;
        case OUT:
        {
            uint8_t operand = pop();
            int combo = calculateComboOperand(operand) % 8;
#ifdef DEBUG_PRINT
            printf("<out> %d\n", combo);
#endif
            outputs.push_back(combo);
            break;
        }
        case BDV:
            registers.B = calculateDivision(registers.A, calculateComboOperand(pop()));
            break;
        case CDV:
            registers.C = calculateDivision(registers.A, calculateComboOperand(pop()));
            break;
        default:
            printf("Unknown opcode <%d>.\n", oper);
            break;
        }
    }

#ifdef DEBUG_PRINT
    printf("Registers: [%d, %d, %d]\n", registers.A, registers.B, registers.C);
    printf("Finished execution.\n");
#endif
    return outputs;
}

typedef struct
{
    int A;
    int B;
    int C;
    std::vector<uint8_t> Opcodes;
} ProgramInput;

ProgramInput build_input(std::string input)
{
    ProgramInput res;
    std::ifstream file(input);
    ScanSequence ss(file);
    file.close();

    // Scan initial values
    ss.consumeString("Register A: ");
    res.A = ss.scanInt();
    ss.consumeString("\nRegister B: ");
    res.B = ss.scanInt();
    ss.consumeString("\nRegister C: ");
    res.C = ss.scanInt();

    ss.consumeString("\n\nProgram: ");
    ss.applyWithDelim(',', [&res](ScanSequence &ss)
                      { res.Opcodes.push_back(ss.scanUInt8()); });

    return res;
}

void part1()
{
    std::string input("input.txt");
    ProgramInput pi = build_input(input);
#ifdef DEBUG_PRINT
    printf("Register A: %d\nRegister B: %d\nRegister C: %d\n", pi.A, pi.B, pi - C);
    std::cout << "Program: ";
    for (auto x : pi.Opcodes)
    {
        std::cout << (int)x << ",";
    }
    std::cout << "\n";
#endif
    // Start the actual program
    Interpreter interpreter(pi.Opcodes, pi.A, pi.B, pi.C);
    auto outputs = interpreter.run();
    std::cout << outputs[0];
    for (int i = 1; i < outputs.size(); i++)
    {
        std::cout << "," << outputs[i];
    }
    std::cout << "\n";
}

static inline bool equal_vec(const std::vector<int> &a, const std::vector<int> &b)
{
    if (a.size() != b.size())
        return false;
    for (int i = 0; i < a.size(); i++)
    {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

void part2()
{
    // Setup the inputs and the goal
    ProgramInput input = build_input("input.txt");
    std::vector<int> goal;
    for (auto x : input.Opcodes)
    {
        goal.push_back((int)x);
        std::cout << "," << (int)x;
    }
    std::cout << std::endl;

    int a = 0;
    while (true)
    {
        Interpreter inter(input.Opcodes, a, input.B, input.C);
        auto res = inter.run();
        if (equal_vec(goal, res))
        {
            std::cout << "Found ! " << a << std::endl;
            break;
        }

        if (a % 1000000 == 0)
        {
            std::cout << "\t\tA = " << a << std::endl;
        }
        a++;
    }
}

int main()
{

    // part1();
    part2();
}