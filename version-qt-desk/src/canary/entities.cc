#include "top/config.h"

#include "canary/entities.h"

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <vector>


namespace domain {

boost::shared_ptr<TaskEntity> TaskEntity::create(std::string& task_name) {
  boost::shared_ptr<TaskEntity> tmp = boost::make_shared<TaskEntity>(TaskEntity());
  tmp->task_name_ = task_name;
  return tmp;
}


}  // namespace
