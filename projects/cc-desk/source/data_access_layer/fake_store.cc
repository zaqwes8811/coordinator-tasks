#include "heart/config.h"

#include "model_layer/entities.h"
#include "model_layer/values.h"

#include <std_own_ext-fix/std_own_ext.h>

#include <cstdlib>


namespace fake_store {
using std::vector;
using std::string;
using namespace entities;

// FIXME: на русском тоже нужно!! и это может стать проблемой
const char* events[] = {
  "A weak_ptr can only be created from a shared_ptr,",
  "and at object construction time no shared_ptr to",
  "Even if you could create a temporary shared_ptr to this, ",
  "it would go out of scope at the end of the constructor, ",
  "and all weak_ptr instances would instantly expire."};

//const char* labels[] = {"v8", "fake"};

Tasks get_all() {
  Tasks model;

  for (int i = 0; i < 5; ++i) {
    string message(events[i]);
    int p = rand() % 10 + 1;
    auto tmp = TaskEntity::create(
          std_own_ext::to_string(p)
          //message
          );

    tmp->m_priority = p;
    model.push_back(tmp);
  }

  return model;
}

std::vector<values::Task> get_all_values() {
  //Tasks model = build_fake_model();

  std::vector<values::Task> v;
  for (int i = 0; i < 5; ++i) {
    int p = rand() % 10 + 1;
    auto tmp = values::Task::create(std_own_ext::to_string(p), p);
    v.push_back(tmp);
  }

  return v;
}

}
