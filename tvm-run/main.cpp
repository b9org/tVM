#include <cstdlib>
#include <fstream>
#include <iostream>

#include "tvm/ExecutionContext.hpp"
#include "tvm/BytecodeParser.hpp"

/// tvm-run's usage string. Printed when run with -help.
static const char* usage =
    "Usage: tvm-run [<option>...] [--] <module> [<arg>...]\n"
    "   Or: tvm-run -help\n"
    "  -debug:        Enable debug code\n"
    "  -jit:          Run in jit mode\n"
    "  -verbose:      Run with verbose printing\n"
    "  -help:         Print this help message\n"
    "--------------------------------------------------------\n"
    "  example:       <path-to-tvm-run>/tvm-run -verbose <path-to-tvm-src>/test/simple_mul.bc" ;

/// The tvm-run program's global configuration.
struct RunConfig {
  tvm::Config tvm;
  const char* moduleName = "";
  bool verbose = false;
};

/// Parse CLI arguments and set up the config.
static bool parseArguments(RunConfig& cfg, const int argc, char* argv[]) {
  std::size_t i = 1;

  for (; i < argc; i++) {
    const char* arg = argv[i];

    if (strcasecmp(arg, "-help") == 0) {
      std::cout << usage << std::endl;
      exit(EXIT_SUCCESS);
    } else if (strcasecmp(arg, "-verbose") == 0) {
      cfg.verbose = true;
      cfg.tvm.verbose = true;
    } else if (strcasecmp(arg, "-debug") == 0) {
      cfg.tvm.debug = true;
    } else if (strcasecmp(arg, "-jit") == 0) {
      cfg.tvm.jit = true;
    } else if (strcmp(arg, "--") == 0) {
      i++;
      break;
    } else if (strcmp(arg, "-") == 0) {
      std::cerr << "Unrecognized option: " << arg << std::endl;
      return false;
    } else {
      break;
    }
  }

  // check for user defined module
  if (i < argc) {
    cfg.moduleName = argv[i++];
  } else {
    std::cerr << "No module name given to tvm-run" << std::endl;
    return false;
  }

  return true;
}

static void run(const RunConfig& cfg) {
  using namespace tvm;

  VirtualMachine  vm{cfg.tvm};
  Module          m;
  BytecodeParser  parser(cfg.verbose);
  std::ifstream   infile(cfg.moduleName);
  std::istream    in(infile.rdbuf());

  m.instructions = parser.getInstructions(in);
  infile.close();

  auto module = std::make_shared<Module>(m);

  vm.load(module);

  StackElement result = vm.run();

  std::cout << result << std::endl;
}

int main(int argc, char* argv[]) {
  RunConfig cfg;

  if (!parseArguments(cfg, argc, argv)) {
    std::cerr << usage << std::endl;
    exit(EXIT_FAILURE);
  }

  try{
    run(cfg);
    exit(EXIT_SUCCESS);
  } catch(const tvm::ByteCodeParserException &e) {
    std::cerr << "Parsing error: " << e.what() << std::endl;
  } catch (const tvm::InterpreterException &e) {
    std::cerr << "Interpreter error: " << e.what() << std::endl;
  } catch (const tvm::CompilationException &e) {
    std::cerr << "Compiling error: " << e.what() << std::endl;
  }

  exit(EXIT_FAILURE);
}
