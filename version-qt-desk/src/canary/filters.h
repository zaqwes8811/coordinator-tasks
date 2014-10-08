// WARNING: danger! всю иерархию фильтров хранить в этой паре файлов! это связано в операцией
//   сравнения по типу.
//
#ifndef FILTERS_H
#define FILTERS_H

#include "canary/entities_and_values.h"

#include <boost/function.hpp>

#include <list>

namespace filters
{

boost::function1<bool, entities::Tasks::value_type> get_check_non_saved();
boost::function1<bool, entities::Tasks::value_type> get_check_contained(const int id);

// могли бы вставляться друг в друга
class Filter {
public:
  // typedefs

  // ctors/..
  virtual ~Filter() { }
  virtual entities::Tasks operator()(entities::Tasks t) = 0;

  // FIXME: хочется обойтись без rtti, пока так.
  //   Проблема в удалении фильтра из цепочки.
  virtual int get_type_id() const = 0;
};
typedef boost::shared_ptr<Filter> FilterPtr;

// на входе весь кеш, на выходе результат собственно, может лучше через SQL?
// Но как легко комбинировать фильтры. Откат к sql может повлиять, а может и нет на архитектуры.
//
// зацепаемся за типы? может еще можно как-то?
class ChainFilters {
public:
  ChainFilters();
  void add(FilterPtr e);

  // FIXME: как удалить то без RTTI? Список то полиморфный
  void remove(FilterPtr e);

  entities::Tasks operator()(entities::Tasks e) const;

private:
  std::list<FilterPtr> l_;
};

}

#endif // FILTERS_H
