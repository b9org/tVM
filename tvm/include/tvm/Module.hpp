#ifndef TVM_MODULE_HPP_
#define TVM_MODULE_HPP_

#include "tvm/instructions.hpp"

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace tvm {

/// An interpreter module.
struct Module {
  std::vector<Instruction> instructions;
};

inline void operator<<(std::ostream& out, const Module& m) {
  for (auto instruction : m.instructions) {
    out << instruction << std::endl;
  }
}

inline bool operator==(const Module& lhs, const Module& rhs) {
  if (lhs.instructions.size() != rhs.instructions.size())
    return false;

  for (std::size_t i = 0; i < lhs.instructions.size(); i ++){
    if (lhs.instructions[i] != rhs.instructions[i])
    return false;
  }

  return true;
}

}  // namespace tvm

#endif  // TVM_MODULE_HPP_
