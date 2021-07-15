#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>


enum class op_code
{
    push,
    swap,
    add,
};

struct op_instruction
{
    op_code code;
    int     operand;
};

static std::optional<std::string> scan_token(std::string& line);


int main()
{
    std::unordered_map<std::string, op_code> const inst_map = {
        {"push", op_code::push},
        {"swap", op_code::swap},
        {"add", op_code::add},
    };

    std::vector<op_instruction> instructions;

    for (std::string line; std::getline(std::cin, line); ) {
        auto op = scan_token(line);
        auto arg = scan_token(line);

        if (!op) {
            continue;
        }
        auto const op_key = *op;

        op_instruction inst;
        inst.code = inst_map.at(op_key);
        if (arg) {
            inst.operand = std::stoi(*arg);
        }

        instructions.push_back(inst);
    }

    std::vector<int> stack;

    for (auto const& inst : instructions) {
        switch (inst.code) {
        case op_code::push:
            stack.push_back(inst.operand);
            break;

        case op_code::swap:
            std::swap(stack.back(), stack[stack.size() - 2]);
            break;

        case op_code::add:
            stack.back() += stack[stack.size() - 2];
            break;
        }

        std::cout << "[" << stack.size() << "] >> ";
        for (auto const& val : stack) {
            std::cout << ' ' << val;
        }
        std::cout << '\n';
    }
}


std::optional<std::string> scan_token(std::string& line)
{
    constexpr char blank = ' ';

    auto const start = line.find_first_not_of(blank);
    if (start == std::string::npos) {
        line.erase();
        return std::nullopt;
    }

    std::string token;
    auto const pos = line.find_first_of(blank, start);
    if (pos == std::string::npos) {
        token = line.substr(start);
        line.erase();
    } else {
        token = line.substr(start, pos - start);
        line.erase(0, pos + 1);
    }

    return token;
}
