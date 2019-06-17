#ifndef TVM_OPERANDSTACK_HPP_
#define TVM_OPERANDSTACK_HPP_

#include <cstring>
#include <iostream>

namespace tvm {

using StackElement = std::int64_t;

class OperandStack {
 public:
  static constexpr std::size_t SIZE = 1000;

  OperandStack() noexcept : top_(&stack_[0]) {
    memset(stack_, 0, sizeof(stack_));
  }

  void reset() { top_ = &stack_[0]; }

  void push(const StackElement &value) {
    *top_ = value;
    ++top_;
  }

  StackElement *pushn(std::size_t n) {
    memset(top_, 0, sizeof(*top_) * n);
    top_ += n;
    return top_;
  }

  StackElement pop() {
    --top_;
    return *top_;
  }

  StackElement *popn(std::size_t n) {
    top_ -= n;
    return top_;
  }

  StackElement *top() { return top_; }

  void drop() { --top_; }

  StackElement peek() const { return *(top_ - 1); }

  StackElement *begin() { return stack_; }

  StackElement *end() { return top_; }

  const StackElement *begin() const { return stack_; }

  const StackElement *end() const { return top_; }

  void restore(StackElement *top) { top_ = top; }


 private:

  StackElement *top_;
  StackElement stack_[SIZE];
};

inline std::ostream &printStack(std::ostream &out, const OperandStack &stack) {
  out << "(stack";

  for (auto e : stack) {
    out << std::endl << "  " << e;
  }

  out << ")" << std::endl;
  return out;
}


}  // namespace tvm

#endif  // TVM_OPERANDSTACK_HPP_
