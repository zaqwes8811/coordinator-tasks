#include "top/config.h"

#include "db_indep.h"
#include "things/values.h"


namespace storages {
void TaskTableQueries::createIfNotExist()
{ createIfNotExistImpl(); }
void TaskTableQueries::drop()
{ dropImpl(); }
void TaskTableQueries::draw(std::ostream& o) const
{ drawImpl(o); }

values::ImmutableTask TaskLifetimeQueries::create(const values::ImmutableTask& v)
{ return createImpl(v); }
void TaskLifetimeQueries::update(const values::ImmutableTask& v)
{ updateImpl(v); }
entities::Tasks TaskLifetimeQueries::get_all() const
{ return get_allImpl(); }
}
