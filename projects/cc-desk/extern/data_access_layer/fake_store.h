#ifndef TEST_HELP_DATA_H
#define TEST_HELP_DATA_H

#include "model_layer/entities.h"

#include <vector>

namespace fake_store {
// Оно просто создает задачи, это не фейковое хранилище, но чем-то похоже
entities::TaskEntities get_all();

std::vector<entities::Task> get_all_values();
}

#endif // TEST_HELP_DATA_H
