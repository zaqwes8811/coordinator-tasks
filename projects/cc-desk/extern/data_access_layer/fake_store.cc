#include "heart/config.h"

#include "model_layer/entities.h"

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

TaskEntities get_all() {
  TaskEntities model;

  for (int i = 0; i < 5; ++i) {
    string message(events[i]);
    int p = rand() % 10 + 1;
    auto tmp = std::make_shared<Task>();//::createEntity();

    tmp->priority = p;
    model.push_back(tmp);
  }

  return model;
}

std::vector<entities::Task> get_all_values() {
  std::vector<entities::Task> v;
  for (int i = 0; i < 5; ++i) {
    int p = rand() % 10 + 1;
    auto tmp = entities::Task();
    v.push_back(tmp);
  }

  return v;
}

}
