add_library(tvm SHARED
	src/ExecutionContext.cpp
	src/VirtualMachine.cpp
	src/instructions.cpp
	src/BytecodeParser.cpp
	src/Compiler.cpp
)

target_include_directories(tvm
	PUBLIC
		include/
)
