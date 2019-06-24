#ifndef TVM_COMPILER_HPP
#define TVM_COMPILER_HPP

#include "instructions.hpp"

#include <sys/mman.h>
#include <vector>
#include <memory>
#include <cstring>

namespace tvm {

struct CompilationException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

/**
 * @brief struct CodeCache, used for managing code cache memory regions where
 * compiled code is written
 */
struct CodeCache {
  /**
   * @brief CodeCache constructor allocates a memory region based on the size of
   * the compiled code to be written
   * @param:in size_t size the size of the code cache to allocate
   */
  CodeCache(size_t size)
      : size_(size) {
    start_ = mmap(0, size_ , PROT_READ | PROT_WRITE | PROT_EXEC,
                  MAP_PRIVATE | MAP_ANON, -1, 0);
    if (start_ == MAP_FAILED)
      throw CompilationException{"Failed to allocate code cache memory."};
  }

  ~CodeCache() noexcept {
    munmap(start_, size_);
  }

  /**
   * @brief writes machine code to code cache
   * @param:in machineCode the encoded instructions to write to code cache
   */

  void writeToCodeCache(const unsigned char * machineCode){
    std::memcpy(start_, machineCode, size_);
  }

  /**
   * @brief get the address of the entry into the code cache
   */
  void * getCodeCacheEntry(){
    return start_;
  }

private:
  void * start_;
  size_t size_;
};

class Compiler {
public:
  Compiler(const std::vector<Instruction> &instructions);

  /**
   * @brief get the address of the entry into the compiled code
   *
   */
  void * getCodeEntry();

private:

  /**
   * @brief generates machine code and handles allocation of
   * code cache and writing machine code to code cache
   *
   */
  void compile();

  /**
   * @brief translates tVM bytecodes into equivalent x86 instructions
   *
   */
  void generateMachineCode();

  std::vector<Instruction> instructions_;
  std::unique_ptr<CodeCache> codeCache_;
  std::vector<unsigned char> machineCode_;

}; // class Compiler

} // namespace tvm


#endif /* TVM_COMPILER_HPP */
