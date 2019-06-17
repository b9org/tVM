#include "tvm/instructions.hpp"

namespace tvm {

std::unordered_map<std::string, OpCode> Instruction::toOpCode = {
    {"int_add",           OpCode::INT_ADD},
    {"int_sub",           OpCode::INT_SUB},
    {"int_mul",           OpCode::INT_MUL},
    {"int_div",           OpCode::INT_DIV},
    {"int_push_constant", OpCode::INT_PUSH_CONSTANT},
    {"end",               OpCode::END},
};

std::unordered_map<OpCode, std::string> Instruction::toString = {
    {OpCode::INT_ADD,           "int_add"},
    {OpCode::INT_SUB,           "int_sub"},
    {OpCode::INT_MUL,           "int_mul"},
    {OpCode::INT_DIV,           "int_div"},
    {OpCode::INT_PUSH_CONSTANT, "int_push_constant"},
    {OpCode::END,               "end"},
};

/// Print a OpCode
std::ostream &operator<<(std::ostream &out, OpCode bc) {
  return out << Instruction::toString[bc];
}

/// Print an Instruction.
std::ostream &operator<<(std::ostream &out, Instruction i) {
  out << "(" << i.opCode();

  if (Instruction::hasImmediate(i.opCode()))
    out << " " << i.immediate();

  return out << ")";
}

}  // namespace tvm
