# tVM

tVM is a very simple stack-based virtual machine meant to educate how a
language VM works.

tVM only supports arithmetic operations. The input to tVM is a .bc file
containing a comma-separated list of bytecodes and the output is the
result of executing the bytecodes (either interpreted or JIT-compiled).

The "JIT compiler" in tVM is a simple x86-64 code generator that translates
tVM bytecodes into corresponding machine encoding.

## Getting started

This page contains some basic instructions to get you started.

### 1. Requirements

* `git`
* `cmake` 3.2 or above
* C/C++ compiler toolchain

### 2. Clone this repository

```
git clone https://github.com/b9org/tVM.git
```

### 3. Create build directory

```
mkdir build \
&& cd build
```

### 4. Configure and build

```
cmake .. \
&& make
```

### 5. Run

From the `build` directory, run:

```
./tvm-run/tvm-run ../test/simple_add.bc
```

The command above would run simple_add.bc in interpreter mode. To JIT-compile
the program and running the compiled code, run with -jit option:

```
./tvm-run/tvm-run -jit ../test/simple_add.bc
```
