#ifndef TEST_HELP_DATA_H
#define TEST_HELP_DATA_H

#include "things/entities_and_values.h"

#include <vector>

namespace fake_store {
// Оно просто создает задачи, это не фейковое хранилище, но чем-то похоже
entities::Tasks get_all();

std::vector<values::ImmutableTask> get_all_values();
}

#endif // TEST_HELP_DATA_H
