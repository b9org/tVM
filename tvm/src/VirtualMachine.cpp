#include <iostream>

#include "tvm/ExecutionContext.hpp"
#include "tvm/VirtualMachine.hpp"

namespace tvm {

VirtualMachine::VirtualMachine(const Config &cfg)
    : cfg_{cfg} {
  if (cfg_.verbose) std::cout << "VM initializing..." << std::endl;
}

VirtualMachine::~VirtualMachine() noexcept {
}

void VirtualMachine::load(std::shared_ptr<const Module> module) {
  module_ = module;
}

StackElement VirtualMachine::run(){
  if (cfg_.jit){
    Compiler compiler(module_->instructions);
    JittedCode jc = reinterpret_cast<JittedCode>(compiler.getCodeEntry());
    return jc();
  }
  ExecutionContext executionContext(*this, cfg_);
  return executionContext.interpret();
}

}  // namespace tvm
