set(LIBRARY "devfixbase")
add_library(${LIBRARY} error/baseexception.h error/interruptedexception.h memory.h platform.h error/timeoutexception.h io/inputstream.h io/source.cpp io/source.h io/sink.cpp io/sink.h io/iotypes.h error/namespace.h strout.h)
set_target_properties(${LIBRARY} PROPERTIES LINKER_LANGUAGE CXX)
target_compile_options(${LIBRARY} PRIVATE ${WARNINGS})
