#ifndef FILTERS_H
#define FILTERS_H

#include "canary/entities.h"

#include <boost/function.hpp>

namespace filters
{

boost::function1<bool, domain::Model::value_type> get_check_non_saved();
}

#endif // FILTERS_H
