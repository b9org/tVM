#include "tvm/Compiler.hpp"

namespace tvm {

Compiler::Compiler(const std::vector<Instruction> &instructions)
  : instructions_(instructions) {
    compile();
}

void Compiler::compile(){
  generateMachineCode();
  codeCache_ = std::make_unique<CodeCache>(machineCode_.size() * sizeof(unsigned char));
  codeCache_->writeToCodeCache(machineCode_.data());
}

void Compiler::generateMachineCode(){
  for (const auto &instruction : instructions_){
      switch (instruction.opCode()){
          case OpCode::INT_ADD:
              machineCode_.push_back(0x58); // pop rax
              machineCode_.push_back(0x5b); // pop rbx
              machineCode_.push_back(0x48); // |
              machineCode_.push_back(0x01); // | add rax, rbx
              machineCode_.push_back(0xd8); // |
              machineCode_.push_back(0x50); // push rax
              break;
          case OpCode::INT_DIV:
              machineCode_.push_back(0x5b); // pop rbx (divisor)
              machineCode_.push_back(0x58); // pop rax (low bits of divident)
              machineCode_.push_back(0x48); // |
              machineCode_.push_back(0x31); // | xor rdx, rdx (rdx = high bits of divident = 0)
              machineCode_.push_back(0xd2); // |
              machineCode_.push_back(0x48); //  |
              machineCode_.push_back(0xf7); //  | div rbx (rdx:rax / rbx)
              machineCode_.push_back(0xf3); //  |
              machineCode_.push_back(0x50); // push rax (rax = quotient)
              break;
          case OpCode::INT_PUSH_CONSTANT:
              machineCode_.push_back(0x6a); // push
              machineCode_.push_back(instruction.immediate()); // constant value
              break;
          case OpCode::END:
              machineCode_.push_back(0x58); // pop rax
              machineCode_.push_back(0xc3); // ret
              return;
          case OpCode::INT_SUB: //todo
          case OpCode::INT_MUL: //todo
              throw CompilationException{"Unimplemented bytecode"};
              break;
          default:
              throw CompilationException{"Unrecognized bytecode"};
              break;
      }
  }
  throw CompilationException{"Missing END bytecode"};
}

void * Compiler::getCodeEntry(){
  return codeCache_->getCodeCacheEntry();
}

} //namespace tvm
