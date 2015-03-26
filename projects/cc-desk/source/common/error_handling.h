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
#include <string>
#include <sstream>
#include <stdexcept>

template <typename T>
std::string toString(T const& value) {
  std::stringstream sstr;
  sstr << value;
  return sstr.str();
}

#ifndef FROM_HERE
#  define FROM_HERE (std::string(__FILE__) + ": " + toString(__LINE__)) + " : "
#endif


#define DCHECK(cond) do { if (!(cond)) throw std::runtime_error("Assert: " + FROM_HERE); } while(0);

/**
// FIXME: may be not put in RAM? After persist view will be updated
//} catch (...) {
  // No way! Can add some task after
  //auto rollback = [this]() {
    //std::remove()
    // FIXME: No can't. Lost user input!
    //this->m_taskCells.pop_back();  // no way!
  //};
  //rollback();
//}
// FIXME: if somewhere failed after persist - then.. state is protect?
*/

#endif
