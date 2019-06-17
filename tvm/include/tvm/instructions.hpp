#ifndef TVM_INSTRUCTIONS_HPP_
#define TVM_INSTRUCTIONS_HPP_

#include <cstdint>
#include <ostream>
#include <unordered_map>

namespace tvm {

using RawOpCode = std::uint8_t;

enum class OpCode : RawOpCode {

  // Generic ByteCodes

  // A special OpCode marking the end of a set of OpCodes
  END = 0x0,

  // Integer opcodes

  // Add two integers
  INT_ADD = 0x1,
  // Subtract two integers
  INT_SUB = 0x2,
  // Multiply two integers
  INT_MUL = 0x3,
  // Divide two integers
  INT_DIV = 0x4,
  // Push a constant
  INT_PUSH_CONSTANT = 0x5

};

using RawInstruction = std::uint32_t;

/// The 24bit immediate encoded in an instruction. Note that parameters are
/// signed values, so special care must be taken to sign extend 24bits to 32.
using Immediate = std::int32_t;

/// A RawInstruction wrapper that will encode and decode instruction opcodes
/// and immediate parameters. The Instruction layout is:
/// ```
/// |0000-0000 0000-0000 0000-0000 0000-0000
/// |---------| opcode (8bits)
///           |-----------------------------| immediate (24bits)
/// ```
///
/// For many ByteCodes, the immediate is unused and left as zero.
class Instruction {
 public:
  constexpr Instruction() noexcept : raw_{0} {}

  constexpr Instruction(RawInstruction raw) noexcept : raw_{raw} {}

  constexpr Instruction(OpCode op) noexcept
      : raw_{RawInstruction(op) << OPCODE_SHIFT} {}

  constexpr Instruction(OpCode op, Immediate p) noexcept
      : raw_{(RawInstruction(op) << OPCODE_SHIFT) | (p & IMMEDIATE_MASK)} {}

  constexpr Instruction &set(OpCode op, Immediate p) noexcept {
    raw_ = (RawInstruction(op) << OPCODE_SHIFT) | (p & IMMEDIATE_MASK);
    return *this;
  }

  /// Encode the opcode
  constexpr Instruction &opCode(OpCode op) noexcept {
    raw_ = (RawInstruction(op) << OPCODE_SHIFT) | (raw_ & IMMEDIATE_MASK);
    return *this;
  }

  /// Decode the opcode
  constexpr OpCode opCode() const noexcept {
    return static_cast<OpCode>(raw_ >> OPCODE_SHIFT);
  }

  /// Encode the immediate
  constexpr Instruction &immediate(Immediate p) noexcept {
    raw_ = (raw_ & OPCODE_MASK) | (p & IMMEDIATE_MASK);
    return *this;
  }

  /// Decode the immediate
  constexpr Immediate immediate() const noexcept {
    auto param = static_cast<Immediate>(raw_ & IMMEDIATE_MASK);
    // Sign extend when top (24th) bit is 1
    if (param & 0x0080'0000) param |= 0xFF00'0000;
    return param;
  }

  constexpr RawInstruction raw() const noexcept { return raw_; }

  constexpr bool operator==(const Instruction rhs) const {
    return raw_ == rhs.raw_;
  }

  constexpr bool operator!=(const Instruction rhs) const {
    return raw_ != rhs.raw_;
  }

 static inline bool hasImmediate(const OpCode &op) {
  switch (op) {
    // 0 immediate
    case OpCode::INT_ADD:
    case OpCode::INT_SUB:
    case OpCode::INT_MUL:
    case OpCode::INT_DIV:
    case OpCode::END:
      return false;
    // 1 immediate
    case OpCode::INT_PUSH_CONSTANT:
    default:
      return true;
  }
}

 public:
  static std::unordered_map<std::string, OpCode> toOpCode;
  static std::unordered_map<OpCode, std::string> toString;

 private:
  static constexpr const RawInstruction OPCODE_SHIFT = 24;
  static constexpr const RawInstruction IMMEDIATE_MASK = 0x00FF'FFFF;
  static constexpr const RawInstruction OPCODE_MASK = ~IMMEDIATE_MASK;

  RawInstruction raw_;
};

/// Print a OpCode
std::ostream &operator<<(std::ostream &out, OpCode bc);

/// Print an Instruction.
std::ostream &operator<<(std::ostream &out, Instruction i);

}  // namespace tvm

#endif  // TVM_INSTRUCTIONS_HPP_
