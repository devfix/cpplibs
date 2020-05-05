add_library(devfixnet inetaddress.h inetaddress.cpp netbuilder.cpp netbuilder.h serversocket.h socketexception.h lnx/lnx_serversocket.cpp lnx/lnx_serversocket.h lnx/lnx_socket.h lnx/lnx_socket.cpp)
set_target_properties(devfixnet PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(devfixnet devfixbase)
target_compile_options(devfixnet PRIVATE ${WARNINGS})
