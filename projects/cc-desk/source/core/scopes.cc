#include "heart/config.h"

#include "scopes.h"

#include <actors_and_workers/arch.h>

namespace scopes {
std::atomic_bool scopes::AppScope::s_isDone{false};

AppScope::AppScope() {
  s_isDone = false;
  Dispatcher::ForkAll();
}

AppScope::~AppScope() {
  Dispatcher::JoinAll();
}
}
