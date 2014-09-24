#include "top/config.h"

#include "canary/entities.h"
#include "canary/pq_dal.h"  // BAD!! too low level
#include "canary/app_core.h"

#include <gtest/gtest.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <memory>

namespace {
using pq_dal::PQConnectionPool;
using busi::AppCore;
using namespace boost;

TEST(AppCore, Create) {

  // make_shared получает по копии - проблема с некопируемыми объектами
  shared_ptr<PQConnectionPool> pool(
        new PQConnectionPool(app::kConnection));
  {
    std::auto_ptr<AppCore> app(AppCore::heapCreate(pool));
    app->clear = true;

  }

  pq_dal::TaskTableQueries q(app::kTaskTableName);
  EXPECT_THROW(q.print(*(pool->get())), pqxx::undefined_table);
}
}  // namespace
