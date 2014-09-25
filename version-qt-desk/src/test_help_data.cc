#include "top/config.h"

#include "canary/entities.h"


namespace test_help_data {
using std::vector;
using std::string;
using namespace boost;
using namespace domain;

const char* events[] = {
  "A weak_ptr can only be created from a shared_ptr,",
  "and at object construction time no shared_ptr to",
  "Even if you could create a temporary shared_ptr to this, ",
  "it would go out of scope at the end of the constructor, ",
  "and all weak_ptr instances would instantly expire."};

//const char* labels[] = {"v8", "fake"};

vector<shared_ptr<TaskEntity> > build_fake_model() {
  vector<shared_ptr<TaskEntity> > model;  

  for (int i = 0; i < 5; ++i) {
    string message(events[i]);
    shared_ptr<TaskEntity> tmp = TaskEntity::create(message);
    model.push_back(tmp);
  }

  return model;
}
}
