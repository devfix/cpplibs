add_library(devfixbase exception/exception.h exception/interruptedexception.h memory.h platform.h exception/timeoutexception.h io/inputstream.h io/source.cpp io/source.h io/sink.cpp io/sink.h io/iotypes.h)
set_target_properties(devfixbase PROPERTIES LINKER_LANGUAGE CXX)
target_compile_options(devfixbase PRIVATE ${WARNINGS})
