#ifndef TEST_HELP_DATA_H
#define TEST_HELP_DATA_H

#include "canary/entities.h"

#include <boost/shared_ptr.hpp>

#include <vector>

namespace test_help_data {
std::vector<boost::shared_ptr<domain::TaskEntity> > build_fake_model();
}

#endif // TEST_HELP_DATA_H
