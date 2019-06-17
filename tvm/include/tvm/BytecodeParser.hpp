#ifndef BYTECODEPARSER_HPP_
#define BYTECODEPARSER_HPP_

#include <iostream>
#include <string>
#include <vector>

#include "instructions.hpp"

namespace tvm {

struct ByteCodeParserException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

/**
 * @brief class BytecodeParser parses tVM instructions in .bc files
 *
 */
class BytecodeParser {
public:
  explicit BytecodeParser(bool verbose = false) noexcept : _verbose(verbose) {}

  /**
   * @brief builds a vector of instructions from the input file
   * @param:in std::istream &in the input stream
   * @return std::vector<Instruction> the vector of instructions
   */
  std::vector<Instruction> getInstructions(std::istream &in);

  /**
   * @brief output instructions that have been read
   * @param:in std::vector<Instruction> &instructionSets the instructions
   */
  void logAllInstructions(const std::vector<Instruction> &instructionSets) const;

private:

  /**
   * @brief parsing helper
   * @param:in std::string &token the token being processed
   */
  void formatToken(std::string &token) const;

  /**
   * @brief parsing helper
   * @param:in std::string line the line being processed
   */
  Instruction processLine(std::string line) const;

private:
  bool _verbose;
};

}// namespace tvm

#endif /* BYTECODEPARSER_HPP_ */
