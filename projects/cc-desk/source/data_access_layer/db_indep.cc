#include "heart/config.h"

#include "db_indep.h"
#include "model_layer/values.h"


namespace storages {
void TaskTableQueries::createIfNotExist()
{ createIfNotExistImpl(); }
void TaskTableQueries::drop()
{ dropImpl(); }
void TaskTableQueries::draw(std::ostream& o) const
{ drawImpl(o); }

values::Task TaskLifetimeQueries::create(const values::Task& v)
{ return createImpl(v); }
void TaskLifetimeQueries::update(const values::Task& v)
{ updateImpl(v); }
entities::Tasks TaskLifetimeQueries::get_all() const
{ return get_allImpl(); }
}
