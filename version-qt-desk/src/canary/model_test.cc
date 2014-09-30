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
#include <adobe/algorithm/for_each.hpp>
#include <loki/ScopeGuard.h>

#include <memory>
#include <ostream>

namespace {
using boost::ref;
using boost::shared_ptr;
using Loki::MakeObjGuard;
using Loki::ScopeGuard;

using pq_dal::PQConnectionPool;
using app_core::AppCore;
using domain::TasksMirror;
using std::cout;
using renders::render_task_store;

TEST(AppCore, Create) {
  // make_shared получает по копии - проблема с некопируемыми объектами
  shared_ptr<PQConnectionPool> pool(
        new PQConnectionPool(app::kConnection));
  {
    std::auto_ptr<AppCore> app_ptr(AppCore::createInHeap(pool));

    // добавляем записи
    TasksMirror data = test_help_data::build_fake_model();

    // как добавить пачкой?
    std::for_each(data.begin(), data.end(), bind(&AppCore::append, ref(*app_ptr), _1));

    //renders::render_task_store(cout, *(app_ptr.get()));
  }

  {
    std::auto_ptr<AppCore> app_ptr(AppCore::createInHeap(pool));
    ScopeGuard _ = MakeObjGuard(*app_ptr, &AppCore::clear_store);

    //renders::render_task_store(cout, *(app_ptr.get()));
  }

  pq_dal::TaskTableQueries q(app::kTaskTableNameRef);
  EXPECT_THROW(q.print(cout, *(pool->get())), pqxx::undefined_table);
}

TEST(AppCore, UpdatePriority) {
  shared_ptr<PQConnectionPool> pool(new PQConnectionPool(app::kConnection));
  {
    std::auto_ptr<AppCore> app_ptr(AppCore::createInHeap(pool));
    ScopeGuard _ = MakeObjGuard(*app_ptr, &AppCore::clear_store);

    // добавляем записи
    TasksMirror data = test_help_data::build_fake_model();
    adobe::for_each(data, bind(&AppCore::append, ref(*app_ptr), _1));
    renders::render_task_store(cout, *app_ptr);

    // Change priority
    data[0]->set_priority(10);
    app_ptr->update(data[0]);
    renders::render_task_store(cout, *app_ptr);
  }

  pq_dal::TaskTableQueries q(app::kTaskTableNameRef);
  EXPECT_THROW(q.print(cout, *(pool->get())), pqxx::undefined_table);
}

TEST(AppCore, SelectionByPriority) {

}

}  // namespace
