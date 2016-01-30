// WARNING: danger! всю иерархию фильтров хранить в этой паре файлов! это связано в операцией
//   сравнения по типу.
//
// TROUBLE: фильтры не такие простные штуки
//
// http://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
// лучше hash_set, set как-то странно проверяет на равенство
// можно hash_map, может так и проще, тогда не нужны свои hash and equal
//
// Logic fail:
//   stable_sort это не то же самое, что отсортировать... - главное, что при пересорт. одинак. элем. сохр. порядок
//   скорее всего дело в том, что строки !!не одинаковые - у них одинаковое начало, но не сами они
//
// Feature:
//   отсортировать по имени и в группах одинаковых по приоритету

#ifndef FILTERS_H
#define FILTERS_H

#include "entities.h"

#include <list>
#include <functional>
#include <unordered_set>

namespace filters
{

std::function<bool(entities::TaskEntity)> is_non_saved();
std::function<bool(entities::TaskEntity)> is_contained(const size_t id);

// могли бы вставляться друг в друга
class Filter {
public:
  // ctors/..
  virtual ~Filter() { }
  virtual entities::TaskEntities operator()(entities::TaskEntities t) = 0;

  // FIXME: хочется обойтись без rtti, пока так.
  //   Проблема в удалении фильтра из цепочки.
  virtual int typeCode() const = 0;
};
typedef gc::SharedPtr<Filter> FilterPtr;

// сырые указатели лучше не передавать.
bool operator==(const Filter& lhs, const Filter& rhs);
bool operator==(FilterPtr lhs, FilterPtr rhs);
//std::size_t hash_value(FilterPtr b);  // пока страшновато, до конца не понял

struct KeyHasher
{
  std::size_t operator()(FilterPtr k) const
  {
    using std::hash;
    return hash<int>()(k->typeCode());
  }
};

struct KeyEqual {
    bool operator()(FilterPtr lhs, FilterPtr rhs) const {
        return lhs == rhs;
    }
};

// на входе весь кеш, на выходе результат собственно, может лучше через SQL?
// Но как легко комбинировать фильтры. Откат к sql может повлиять, а может и нет на архитектуры.
//
// зацепаемся за типы? может еще можно как-то?
// FIXME: Фильтры должны быть линейными, а значить переставляемыми, но как доказать?
class ChainFilters {
public:
  ChainFilters();
  void add(FilterPtr e);

  /**
    \fixme как удалить то без RTTI? Список то полиморфный
  */
  void remove(FilterPtr e);

  /**
    \attention Logical troubles with filter composition
  */
  entities::TaskEntities operator()(entities::TaskEntities e) const;

private:
  // FIXME: можно вообще тупо массив, фильтров все равно не 100500
  std::unordered_set<FilterPtr, KeyHasher, KeyEqual> m_s;  // need own hasher
};

class DoneFilter : public Filter {
public:
  entities::TaskEntities operator()(entities::TaskEntities e);
  int typeCode() const;
};

class SortByPriorityFilter : public Filter {
public:
  entities::TaskEntities operator()(entities::TaskEntities e);
  int typeCode() const;
};

class SortByTaskName : public Filter {
public:
    entities::TaskEntities operator()(entities::TaskEntities e);
    int typeCode() const;
};
}

#endif // FILTERS_H
