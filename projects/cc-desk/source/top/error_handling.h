#ifndef TOP_ERROR_HANDLING_H_
#define TOP_ERROR_HANDLING_H_
// exceptions and DbC

// Как быть с Qt - она собирается без исключений!!

#include <cassert>

#define DCHECK(cond) assert(cond);

#ifndef FROM_HERE
#  define FROM_HERE ""
#endif

#endif
