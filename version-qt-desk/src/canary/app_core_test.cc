#include "top/config.h"

#include "canary/entities.h"
#include "canary/pq_queries.h"  // BAD!! too low level
#include "canary/app_core.h"
#include "test_help_data.h"
#include "canary/renders.h"

#include <gtest/gtest.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <memory>
#include <ostream>

namespace {
using pq_dal::PQConnectionPool;
using app_core::AppCore;
using domain::TasksMirror;
using namespace boost;
using std::cout;
using ::renders::render_task_store;

TEST(AppCore, Create) {

  // make_shared получает по копии - проблема с некопируемыми объектами
  shared_ptr<PQConnectionPool> pool(
        new PQConnectionPool(app::kConnection));
  {
    std::auto_ptr<AppCore> app_ptr(AppCore::heapCreate(pool));

    // добавляем записи
    TasksMirror data = test_help_data::build_fake_model();

    // как добавить пачкой?
    std::for_each(data.begin(), data.end(), bind(&AppCore::append, ref(*app_ptr), _1));

    renders::render_task_store<AppCore>(cout, *(app_ptr.get()));
  }

  {
    std::auto_ptr<AppCore> app_ptr(AppCore::heapCreate(pool));
    app_ptr->clear = true;
    renders::render_task_store<AppCore>(cout, *(app_ptr.get()));
  }

  pq_dal::TaskTableQueries q(app::kTaskTableName);
  EXPECT_THROW(q.print(cout, *(pool->get())), pqxx::undefined_table);
}

TEST(AppCore, UpdatePriority) {

}

TEST(AppCore, SelectionByPriority) {

}

}  // namespace
