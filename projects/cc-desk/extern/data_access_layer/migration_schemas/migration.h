#ifndef DAL_MIGRATION_H_
#define DAL_MIGRATION_H_

namespace migration {
/**
  \case Exist file, need update software
*/
class Versions {
public:
  void makeV0() { }

private:
  // read current db version, check hardcoded
};
}

#endif
