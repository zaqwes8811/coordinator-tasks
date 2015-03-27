#ifndef STD_OWN_EXT_H_
#define STD_OWN_EXT_H_

#include <boost/lexical_cast.hpp>

#include <map>
#include <string>
#include <sstream>

#ifndef FROM_HERE
#define FROM_HERE ""
#endif

namespace std_own_ext {
template <typename K, typename V>
V at(const std::map<K, V>& param, const K& val) {
  // http://stackoverflow.com/questions/3311633/nested-templates-with-dependent-scope
  typename std::map<K, V>::const_iterator it = param.find(val);
  if (it != param.end()) return it->second;
  else throw std::invalid_argument(FROM_HERE);
}

template <typename K, typename V>
V at(const std::map<K, V>& param, const char* const val) {
  typename std::map<K, V>::const_iterator it = param.find(std::string(val));
  if (it != param.end()) return it->second;
  else throw std::invalid_argument(FROM_HERE);
}

template <typename K, typename V>
bool contain(const std::map<K, V>& param, const K& val) {
  typename std::map<K, V>::const_iterator it = param.find(val);
  return it != param.end();
}

template <typename K, typename V>
bool contain(const std::map<K, V>& param, const char* const val) {
  typename std::map<K, V>::const_iterator it = param.find(std::string(val));
  return it != param.end();
}


template<typename V>
struct cache
{
public:
  explicit cache(const V& _value) :
    value(_value)
    , stale(true) // !!
  { }
  V value;

  void evict()
  { stale = true; }

  bool isStale() const
  { return stale; }

private:
  bool stale;
};

// Lavavej homework
// http://channel9.msdn.com/Series/C9-Lectures-Stephan-T-Lavavej-Standard-Template-Library-STL-/C9-Lectures-Stephan-T-Lavavej-Standard-Template-Library-STL-2-of-n

// After CUDA
// Using stable_partition, but how be with predicate - it must be stateless
//   how check buy mask.
//
// DANGER: length not follow
template<class InputIterator, class OutputIterator>
OutputIterator compact(InputIterator in_first, InputIterator in_last,
                       InputIterator mask_first,
                       OutputIterator out_first) {

  // DANGER: No check precond.
  while (in_first != in_last) {
    if (*mask_first) {
      *out_first = *in_first;
      ++out_first;  //
    }

    // http://stackoverflow.com/questions/20108022/incrementing-back-inserter-is-optional
    //++out_first;  // look like bug - logic + ASan - work if put back_inserter
    ++in_first;
    ++mask_first;
  }
  return out_first;
}

// After
// http://channel9.msdn.com/Events/GoingNative/2013/Cpp-Seasoning

// FIXME: iterator for big raw arrays >> max(vector)


// On move from Boost to C++11
// http://stackoverflow.com/questions/8065413/stdlexical-cast-is-there-such-a-thing
template <typename T>
T lexical_cast(const std::string& str)
{
    /*T var;
      // FIXME: troubles if string with spaces
    std::istringstream iss;
    iss.str(str);
    iss >> var;
    */
    // deal with any error bits that may have been set on the stream
    return boost::lexical_cast<T>(str);
}

template <typename T>
std::string to_string(T const& value) {
  std::stringstream sstr;
  sstr << value;
  return sstr.str();
}
}  // space

#endif
