#include "heart/config.h"

#include "db_indep.h"
#include "model_layer/entities.h"


namespace storages {
void TaskTableQueries::createIfNotExist()
{ createIfNotExistImpl(); }
void TaskTableQueries::drop()
{ dropImpl(); }
void TaskTableQueries::draw(std::ostream& o) const
{ drawImpl(o); }

entities::TaskValue TaskLifetimeQueries::create(const entities::TaskValue& v)
{ return createImpl(v); }
void TaskLifetimeQueries::update(const entities::TaskValue& v)
{ updateImpl(v); }
entities::TaskEntities TaskLifetimeQueries::get_all() const
{ return get_allImpl(); }
}
