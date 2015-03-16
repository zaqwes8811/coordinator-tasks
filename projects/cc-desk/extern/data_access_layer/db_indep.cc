#include "heart/config.h"

#include "db_indep.h"
#include "model_layer/entities.h"


namespace storages {
void TaskTableQueries::registerBeanClass()
{ createIfNotExistImpl(); }
void TaskTableQueries::drop()
{ dropImpl(); }
void TaskTableQueries::draw(std::ostream& o) const
{ drawImpl(o); }

entities::Task TaskLifetimeQueries::create(const entities::Task& v)
{ return do_create(v); }
void TaskLifetimeQueries::update(const entities::Task& v)
{ do_update(v); }
entities::TaskEntities TaskLifetimeQueries::loadAll() const
{ return do_loadAll(); }
}
