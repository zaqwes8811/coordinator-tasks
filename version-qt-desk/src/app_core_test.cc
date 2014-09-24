#include "top/config.h"

#include "canary/entities.h"
#include "canary/lower_level.h"

#include <gtest/gtest.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <memory>

namespace {
using lower_level::PQConnectionPool;
using domain::AppCore;
using namespace boost;

TEST(AppCore, Create) {
  // make_shared получает по копии - проблема с некопируемыми объектами
  shared_ptr<PQConnectionPool> pool(
        new PQConnectionPool(app::kConnection));
}
}  // namespace
