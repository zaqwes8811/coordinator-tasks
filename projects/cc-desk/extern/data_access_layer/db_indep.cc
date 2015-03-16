#include "heart/config.h"

#include "db_indep.h"
#include "model_layer/entities.h"


namespace storages {
void TaskTableQueries::registerBeanClass()
{ do_registerBeanClass(); }
void TaskTableQueries::drop()
{ do_drop(); }

entities::Task TaskLifetimeQueries::copyBean(const entities::Task& v)
{ return do_copyBean(v); }
void TaskLifetimeQueries::update(const entities::Task& v)
{ do_update(v); }
entities::TaskEntities TaskLifetimeQueries::loadAll() const
{ return do_loadAll(); }
}
