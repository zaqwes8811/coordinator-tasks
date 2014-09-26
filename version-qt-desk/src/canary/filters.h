#ifndef FILTERS_H
#define FILTERS_H

#include "canary/entities.h"

#include <boost/function.hpp>

namespace filters
{

boost::function1<bool, domain::TasksMirror::value_type> get_check_non_saved();
boost::function1<bool, domain::TasksMirror::value_type> get_check_contained(const int id);
}

#endif // FILTERS_H
