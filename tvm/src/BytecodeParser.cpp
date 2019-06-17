#include <fstream>
#include <sstream>
#include <algorithm>

#include "tvm/BytecodeParser.hpp"

namespace tvm {

void BytecodeParser::formatToken(std::string &token) const{
  // trim the last ','
  if (token.back() == ',') {
    token.pop_back();
  }
  // change to all lower case
  std::transform(token.begin(), token.end(), token.begin(), ::tolower);
}

Instruction BytecodeParser::processLine(std::string line) const{
  constexpr char delimiter = ' ';
  OpCode op = OpCode::END;
  std::int32_t immediate = 0;
  std::string token;

  formatToken(line);

  std::stringstream tokenStream(line);
  std::getline(tokenStream, token, delimiter);

  try {
     op = Instruction::toOpCode.at(token);

     if (!Instruction::hasImmediate(op)) {
       return Instruction(op);
     } else {
       std::getline(tokenStream, token, delimiter);
       immediate = std::stoi(token);
       return Instruction(op, immediate);
     }
  } catch (const std::out_of_range &e) {
    std::cout << "Invalid OpCode: "<< e.what() << std::endl;
  } catch (const std::invalid_argument &e) {
	 std::cout << "Invalid immediate: " << e.what() << std::endl;
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }

  line.insert(0, "Invalid bytecode: ");
  throw ByteCodeParserException{line};
}

std::vector<Instruction> BytecodeParser::getInstructions(std::istream &in) {
  std::vector<Instruction> instructions;
  std::string line;

  while (std::getline(in, line)) {
    instructions.push_back(processLine(line));
  }

  if (_verbose) {
    logAllInstructions(instructions);
  }

  return instructions;
}

void BytecodeParser::logAllInstructions(const std::vector<Instruction> &instructionSets) const {
  for (auto instr : instructionSets) {
    std::cout << instr << std::endl;
  }
}

}// namespace tvm
