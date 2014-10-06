// WARNING: danger! всю иерархию фильтров хранить в этой паре файлов! это связано в операцией
//   сравнения по типу.
//
#ifndef FILTERS_H
#define FILTERS_H

#include "canary/entities_and_values.h"

#include <boost/function.hpp>

namespace filters
{

boost::function1<bool, entities::Tasks::value_type> get_check_non_saved();
boost::function1<bool, entities::Tasks::value_type> get_check_contained(const int id);

// могли бы вставляться друг в друга
class Filter {
public:
  // typedefs
  typedef boost::shared_ptr<Filter> FilterPtr;

  // ctors/..
  virtual ~Filter() { }
  virtual entities::Tasks operator()(entities::Tasks t) = 0;

  // FIXME: хочется обойтись без rtti, пока так.
  //   Проблема в удалении фильтра из цепочки.
  virtual int get_type_id() const = 0;
};

}

#endif // FILTERS_H
