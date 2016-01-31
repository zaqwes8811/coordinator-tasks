/// Spec.:
// - Два типа клиентов с тегами в контейнере
// - C++98
// - Без базового класса. Без указателей. Без циклов.
//
// Или так есть классы без базового. Есть списки из этих конкр. типов
// Хотелось бы их обрабатывать алгоритмами стандартной библиотки.
// - Делать выборки
//
// Т.е. разделение по типам и по тегам. В каждом типе будет обязательный тег.
// А если обязательный тег одного типа будет в тегах другого? Как разделить?
//
// Karantin:
// - реализовать не через VI (NVI, ...)

/// Impl.:
// Вообще проблема в том, как положить в контейнер разные типы, потом их разделить при запросе.
// Container<object_t> - тип то один!
//
// Можно было бы хранить в нескольких контейнерах одинаковые типы,
// а потом их объединять для обработки. Но опять же одинаковые... Скользко. Можно попутать.
//
// Узнать тип объекта:
// http://stackoverflow.com/questions/1986418/typeid-and-typeof-in-c
// http://en.wikibooks.org/wiki/C%2B%2B_Programming/RTTI
//
// Karantin:
// Может что-то типа GetMainTag()? BAD!!

#include <vector>
#include <iostream>
#include <algorithm>
#include <list>

#include <gtest/gtest.h>

#include <functional>

using std::cout;
using std::vector;
using std::for_each;
using std::list;
using std::bind;

//TODO: http://www.informit.com/articles/article.aspx?p=412354&seqNum=4
//TODO: http://stackoverflow.com/questions/10555566/is-there-any-difference-between-c11-stdbind-and-boostbind

void five_args(int i1, int i2, int i3)
{
  cout << i1 << i2 << i3 << '\n';
}

TEST(BindWithBoost, App) {
  using namespace std::placeholders;
  (bind(&five_args, _1, _2,_3))(0, 1, 1);
}

class caller {
public:
  caller(int a, int b) : second_elem(a), end_elem(b) {}
  void operator()(const int value) const {
    five_args(value, second_elem, end_elem);
  }
private:
  const int second_elem;
  const int end_elem;
};

TEST(BindWithoutBoost, App) {
  using std::for_each;

  vector<int> a;
  a.push_back(0);
  a.push_back(9);
  a.push_back(0);

  for_each(a.begin(), a.end(),
           caller(1, 8));
           //bind(five_args, _1, _2, _3)(9, 8));
}

TEST(BoostLambdas, App) {
  using namespace std::placeholders;
  // http://www.boost.org/doc/libs/1_31_0/libs/lambda/doc/lambda_docs_as_one_file.html

  list<int> v(10);
  int i = 9;
  //for_each(v.begin(), v.end(), (_1 = i)(i));

  //for_each(v.begin(), v.end(), (cout << (i + _1))(i));
}
