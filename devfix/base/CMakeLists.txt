add_library(devfixbase error/baseexception.h error/interruptedexception.h memory.h platform.h error/timeoutexception.h io/inputstream.h io/source.cpp io/source.h io/sink.cpp io/sink.h io/iotypes.h error/namespace.h)
set_target_properties(devfixbase PROPERTIES LINKER_LANGUAGE CXX)
target_compile_options(devfixbase PRIVATE ${WARNINGS})
