#ifndef CORE_SCOPES_H_
#define CORE_SCOPES_H_

#include <atomic>

namespace scopes {
class AppScope {
public:
  AppScope() { s_isDone = false; }

  void setToDone() { s_isDone = true; }

  bool isClosed() { return s_isDone; }

private:
  static std::atomic_bool s_isDone;
};
}

#endif
