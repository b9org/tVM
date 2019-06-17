#ifndef TVM_EXECUTIONCONTEXT_HPP_
#define TVM_EXECUTIONCONTEXT_HPP_

#include "tvm/OperandStack.hpp"
#include "tvm/VirtualMachine.hpp"

#include <iostream>

namespace tvm {

/**
 * @brief class ExecutionContext handles interpreting bytecodes for the VM
 *
 */
class ExecutionContext {
 public:
  ExecutionContext(VirtualMachine &virtualMachine, const Config &cfg);

  /**
   * @brief execute the bytecodes using the interpreter loop
   *
   */
  StackElement interpret();

  /**
   * @brief pop from stack
   * @return StackElement the value at top of the stack
   */
  StackElement pop();

  /**
   * @brief push into stack
   * @param:in StackElement e the value to push
   */
  void push(StackElement e);

  /**
   * @brief get the operand stack
   * @return const operandStack &
   */
  const OperandStack &stack() const { return stack_; }

  /**
   * @brief get the VirtualMachine for this execution context
   * @return VirtualMachine *
   */
  VirtualMachine *virtualMachine() const { return virtualMachine_; }

  friend std::ostream &operator<<(std::ostream &stream,
                                  const ExecutionContext &ec);

 private:
  friend class VirtualMachine;

  /**
   * interpreter loop helpers
   */

  void doIntAdd();

  // void doIntSub(); todo

  void doIntMul();

  // void doIntDiv(); todo

  void doIntPushConstant(Immediate value);


  /*
   * Private fields
   */
  OperandStack stack_;
  const Config *cfg_;
  VirtualMachine *virtualMachine_;
};

}  // namespace tvm

#endif  // TVM_EXECUTIONCONTEXT_HPP_
