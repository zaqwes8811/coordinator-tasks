#ifndef CORE_SCOPES_H_
#define CORE_SCOPES_H_

#include <atomic>

namespace scopes {
class AppScope {
public:
  AppScope();
  ~AppScope();

private:
  void setToDone() { s_isDone = true; }
  bool isClosed() { return s_isDone; }
  static std::atomic_bool s_isDone;
};
}

#endif
