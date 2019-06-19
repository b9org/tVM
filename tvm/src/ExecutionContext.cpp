#include "tvm/ExecutionContext.hpp"
#include "tvm/VirtualMachine.hpp"

namespace tvm {

ExecutionContext::ExecutionContext(VirtualMachine &virtualMachine,
                                   const Config &cfg)
    : virtualMachine_(&virtualMachine),
      cfg_(&cfg) {
}

StackElement ExecutionContext::interpret() {

  const Instruction *instructionPointer = virtualMachine_->module()->instructions.data();
  StackElement result;

  while (instructionPointer) {
    switch (instructionPointer->opCode()) {
      case OpCode::INT_ADD:
        doIntAdd();
        break;
      case OpCode::INT_MUL:
        doIntMul();
        break;
      case OpCode::INT_PUSH_CONSTANT:
        doIntPushConstant(instructionPointer->immediate());
        break;
      case OpCode::END:
        result = pop();
        return result;
      case OpCode::INT_SUB: //todo
      case OpCode::INT_DIV: //todo
        throw InterpreterException{"Unimplemented bytecode"};
        break;
      default:
        throw InterpreterException{"Unknown bytecode"};
        break;
    }
    instructionPointer++;
  }
  throw InterpreterException{"Missing END bytecode"};
}

void ExecutionContext::push(StackElement value) { stack_.push(value); }

StackElement ExecutionContext::pop() { return stack_.pop(); }

void ExecutionContext::doIntAdd() {
  auto right = pop();
  auto left = pop();
  push((left + right));
}

//void ExecutionContext::doIntSub() {
  //todo
//}

void ExecutionContext::doIntMul() {
  auto right = pop();
  auto left = pop();
  push(left * right);
}

//void ExecutionContext::doIntDiv() {
  //todo
//}

void ExecutionContext::doIntPushConstant(Immediate value) {
  push(static_cast<std::int64_t>(value));
}

}  // namespace tvm
