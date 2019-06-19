#ifndef TVM_VIRTUALMACHINE_HPP_
#define TVM_VIRTUALMACHINE_HPP_

#include "tvm/Module.hpp"
#include "tvm/OperandStack.hpp"
#include "tvm/instructions.hpp"
#include "tvm/Compiler.hpp"

#include <cstring>
#include <ostream>
#include <string>
#include <vector>
#include <memory>

namespace tvm {

class ExecutionContext;
class VirtualMachine;

struct Config {
  bool debug = false;              //< Enable debug code
  bool verbose = false;            //< Enable verbose printing and tracing
  bool jit = false;
};

inline std::ostream &operator<<(std::ostream &out, const Config &cfg) {
  out << std::boolalpha;
  out << "debug:        " << cfg.debug;
  out << std::noboolalpha;
  return out;
}

struct InterpreterException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

typedef StackElement (* JittedCode)();

class VirtualMachine {
 public:
  VirtualMachine(const Config &cfg);

  ~VirtualMachine() noexcept;

  /// Load a module into the VM.
  void load(std::shared_ptr<const Module> module);

  StackElement run();

  const std::shared_ptr<const Module> &module() { return module_; }

  const Config &config() { return cfg_; }

 private:

  Config cfg_;
  std::shared_ptr<const Module> module_;
};

}  // namespace tvm

#endif  // TVM_VIRTUALMACHINE_HPP_
