//
// Created by Praktikum on 05.05.2020.
//

#pragma once

#include "../../base/platform.h"

#if PLATFORM_WINDOWS == 1

#include <atomic>
#include "../socket.h"
#include "../inetaddress.h"
#include "../../base/io/source.h"
#include "../../base/io/sink.h"


// forward declaration for external builder
namespace devfix::net
{
struct [[maybe_unused]] netbuilder;
}

namespace devfix::net::win
{

// forward declaration for accept() in win_serversocket
struct /*[[maybe_unused]]*/ win_serversocket;

struct win_socket : public socket
{
  friend struct net::netbuilder;
  friend struct win_serversocket;

  ~win_socket() override;

  [[nodiscard]] const inetaddress &get_local_address() const noexcept override;
  [[nodiscard]] const inetaddress &get_remote_address() const noexcept override;

  [[nodiscard]] base::io::inputstream &get_inputstream() const noexcept override;
  [[nodiscard]] base::io::outputstream &get_outputstream() const noexcept override;

  void set_interrupted(bool interrupted) noexcept override;
  [[nodiscard]] bool interrupted() const noexcept override;

  void set_timeout(timeout_t timeout) noexcept override;
  [[nodiscard]] timeout_t get_timeout() const noexcept override;
};

} // namespace devfix::net::win

#endif
