#include "top/config.h"

#include "things/entities_and_values.h"
#include "top/common.h"

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
          common::to_string(p)
          //message
          );

    tmp->setPriority(p);
    model.push_back(tmp);
  }

  return model;
}

std::vector<values::ImmutableTask> get_all_values() {
  //Tasks model = build_fake_model();

  std::vector<values::ImmutableTask> v;
  for (int i = 0; i < 5; ++i) {
    int p = rand() % 10 + 1;
    auto tmp = values::ImmutableTask::create(common::to_string(p), p);
    v.push_back(tmp);
  }

  return v;
}

}
