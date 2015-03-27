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

/**
  \brief Soft assertion
*/
class fatal_error : public std::runtime_error {
public:
  fatal_error(const std::string& m) : std::runtime_error(m)
  { }
};

/**
  \brief Really rear

  \fixme may be not put in RAM? After persist view will be updated
  } catch (...) {
    No way! Can add some task after
    auto rollback = [this]() {
    //std::remove()
    // FIXME: No can't. Lost user input!
    this->m_taskCells.pop_back();  // no way!
    };
    rollback();
  }
 \fixme  if somewhere failed after persist - then.. state is protect?

  \attention Really looks like marshaling errors only throuth futures<>
*/
class infrastructure_error : public std::runtime_error {
public:
  infrastructure_error(const std::string& m) : std::runtime_error(m)
  { }
};

class unknown_error : public std::runtime_error {
public:
  unknown_error(const std::string& m) : std::runtime_error(m)
  { }
};



#ifndef FROM_HERE
#  define FROM_HERE (std::string(__FILE__) + ": " + toString(__LINE__)) + " : "
#endif


#define DCHECK(cond) do { if (!(cond)) throw fatal_error("Assert: " + FROM_HERE); } while(0);



#endif
