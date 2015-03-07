#ifndef DB_INDEP_H_
#define DB_INDEP_H_

namespace storages {
struct TablePositions {
  const static int kId = 0;
  const static int kTaskName = 1;
  const static int kPriority = 2;
  const static int kDone = 3;
};
}

#endif
