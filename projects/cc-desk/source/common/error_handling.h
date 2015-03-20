/**
  даже если исключение брошено при
  сохранении, а элемент добавлен, то можно потом сохр.
  но! лучше сохранить! так мы копим несохраненные данные!

  нет, лучше транзакцией и по много не сохранять.
  будет зависеть от производительности.
  но лучше работать с

  add to container

  FIXME: может лучше исключение?
*/

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
