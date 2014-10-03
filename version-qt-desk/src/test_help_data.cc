#include "top/config.h"

#include "canary/entities_and_values.h"


namespace test_help_data {
using std::vector;
using std::string;
using namespace boost;
using namespace entities;

// FIXME: на русском тоже нужно!! и это может стать проблемой
const char* events[] = {
  "A weak_ptr can only be created from a shared_ptr,",
  "and at object construction time no shared_ptr to",
  "Even if you could create a temporary shared_ptr to this, ",
  "it would go out of scope at the end of the constructor, ",
  "and all weak_ptr instances would instantly expire."};

//const char* labels[] = {"v8", "fake"};

Tasks build_fake_model() {
  Tasks model;

  for (int i = 0; i < 5; ++i) {
    string message(events[i]);
    Tasks::value_type tmp = TaskEntity::create(message);
    model.push_back(tmp);
  }

  return model;
}
}
