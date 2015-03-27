// http://channel9.msdn.com/Events/GoingNative/2013/Inheritance-Is-The-Base-Class-of-Evil
//
// Trouble: Как понял проблема в том, чтобы можно было залить в контейнер разные типы
//   но не пользоватья указателями.
//
// Trouble: А если уже есть классы без иерархии, но нужно сделать что-то общее
//   как в фотошопе. Иерархию вводить поздно и объекты разнородные.
//
// Trouble: shared_ptr его можно хранить в полиморфном контейнере, но не хочется
//   нужна аллокация в куче и такие указатели не лучше глоб. переменных
//
// Trouble: десереализация - есть немного в комментах

#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include <algorithm>

#include <typeinfo>  // не хотелось бы включать, но похоже нужно

#include <gtest/gtest.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

using std::ostream;
using std::string;
using std::endl;
using std::cout;
using std::vector;

// C++11
using std::move;

namespace step0 {
using object_t = int;  // step 2
}

namespace step1 {
void draw(const int& x, ostream& out, size_t position)  // object_t -> int and move here
{ out << string(position, ' ') << x << endl; }

// step 3: remove
class object_t {
public:
  object_t(const int& x) : self_(x)
  {}

  friend void draw(const object_t &x, ostream &out, size_t position)
  { draw(x.self_, out, position); }

private:
  int self_;
};
}

namespace step2 {
void draw(const int& x, ostream& out, size_t position)  // object_t -> int and move here
{ out << string(position, ' ') << x << endl; }

class object_t {
public:
  object_t(const int& x) : self_(new int_model_t(x))
  { }

  object_t(const object_t& x) : self_(new int_model_t(*x.self_))
  { }  // если оставить только копирующий констр. компилятор (gcc 4.7) заругается

  object_t& operator=(const object_t& x)
  { object_t tmp(x);
    self_ = std::move(tmp.self_);
    //std::swap(self_, tmp.self_);  // also compiled, but may be not exc. safe
    return *this; }

  friend void draw(const object_t &x, ostream &out, size_t position)
  { x.self_->draw_(out, position); }  // разрешаем доступ к закрытым частям

private:
  struct int_model_t {
    int_model_t(const int& x) : data_(x) { }
    void draw_(ostream& out, size_t position) const
    {draw(data_, out, position);}

    int data_;
  };
  std::unique_ptr<int_model_t> self_;
};
}

namespace step3 {
  void draw(const int& x, ostream& out, size_t position)  // object_t -> int and move here
  { out << string(position, ' ') << x << endl; }

class object_t {
public:
  object_t(const int& x) : self_(new int_model_t(x))
  { cout << "ctor\n";}

  object_t(const object_t& x) : self_(new int_model_t(*x.self_))
  { cout << "copy\n";}  // если оставить только копирующий констр. компилятор (gcc 4.7) заругается

  // Speed up
  object_t(object_t&&) noexcept = default;

  object_t& operator=(const object_t& x)
  { object_t tmp(x);
    *this = std::move(tmp);  // if no move assign progr. is failed
    //std::swap(self_, tmp.self_);  // also compiled, but may be not exc. safe
    return *this; }
  object_t& operator=(object_t&&) noexcept = default;  // Need it!


  friend void draw(const object_t &x, ostream &out, size_t position)
  { x.self_->draw_(out, position); }  // разрешаем доступ к закрытым частям

private:
  struct int_model_t {
    int_model_t(const int& x) : data_(x) { }
    void draw_(ostream& out, size_t position) const
    {draw(data_, out, position);}

    int data_;
  };
  std::unique_ptr<int_model_t> self_;
};
}

namespace step4 {
void draw(const int& x, ostream& out, size_t position)  // object_t -> int and move here
{ out << string(position, ' ') << x << endl; }

void draw(const string& x, ostream& out, size_t position)  // object_t -> int and move here
{ out << string(position, ' ') << x << endl; }

// "Don't allow polymorphism to complicate client code"
// и все равно много обращений к куче. Но суть думаю в том, что пользователю не нужно
// будет думать об этом.
class object_t {
public:
  object_t(const string& x) : self_(new string_model_t(move(x)))
  {}

  object_t(const int& x) : self_(new int_model_t(move(x)))
  { cout << "ctor\n";}

  // Not compiled
  //object_t(const object_t& x) : self_(new int_model_t(*x.self_))
  object_t(const object_t& x) : self_(x.self_->copy_())
  { cout << "copy\n";}  // если оставить только копирующий констр. компилятор (gcc 4.7) заругается

  // Speed up
  object_t(object_t&&) noexcept = default;

  object_t& operator=(const object_t& x)
  { object_t tmp(x);
    *this = std::move(tmp);  // if no move assign progr. is failed
    //std::swap(self_, tmp.self_);  // also compiled, but may be not exc. safe
    return *this; }
  object_t& operator=(object_t&&) noexcept = default;  // Need it!


  friend void draw(const object_t &x, ostream &out, size_t position)
  { x.self_->draw_(out, position); }  // разрешаем доступ к закрытым частям

private:
  struct concept_t {
    virtual ~concept_t() = default;
    virtual concept_t* copy_() const = 0;
    virtual void draw_(ostream& out, size_t position) const = 0;
  };

  struct string_model_t : concept_t {
    string_model_t(const string& x) : data_(move(x)) { }
    void draw_(ostream& out, size_t position) const
    {draw(data_, out, position);}

    concept_t* copy_() const { return new string_model_t(*this); }

    string data_;
  };

  struct int_model_t : concept_t {
    int_model_t(const int& x) : data_(move(x)) { }
    void draw_(ostream& out, size_t position) const
    {draw(data_, out, position);}

    concept_t* copy_() const { return new int_model_t(*this); }

    int data_;
  };

  // std::unique_ptr<int_model_t> self_;
  std::unique_ptr<concept_t> self_;
};
}

namespace step5 {

/// BAD: If no-C++11 много копирований. И много обращений к куче
// Хотя.. что выделяется в куче? Хендлы или сами объекты?
//
// Если используется полиморфизм, то и сами объекты создаются в куче!
template<typename T>
void draw(const T& x, ostream& out, size_t position)  // object_t -> int and move here
{ out << string(position, ' ') << x << endl; }

//template<typename T>  // не тут
class object_t {
public:
  // шаблонный конструктор
  // http://ldmitrieva.blogspot.ru/2010/11/blog-post_12.html
  template<typename T>
  object_t(T x) : self_(new model<T>(move(x)))  // by value/ специализируем шаблонный класс
  {}

  // Not compiled
  //object_t(const object_t& x) : self_(new int_model_t(*x.self_))
  object_t(const object_t& x) : self_(x.self_->copy_())
  {
    //cout << "copy\n";
  }  // если оставить только копирующий констр. компилятор (gcc 4.7) заругается

  // Speed up
  object_t(object_t&&) noexcept = default;

  object_t& operator=(const object_t& x)
  { object_t tmp(x);
    *this = std::move(tmp);  // if no move assign progr. is failed
    //std::swap(self_, tmp.self_);  // also compiled, but may be not exc. safe
    return *this; }
  object_t& operator=(object_t&&) noexcept = default;  // Need it!


  friend void draw(const object_t &x, ostream &out, size_t position)
  {
    x.self_->draw_(out, position);
  }  // разрешаем доступ к закрытым частям

private:
  struct concept_t {
    virtual ~concept_t() = default;
    virtual concept_t* copy_() const = 0;
    virtual void draw_(ostream& out, size_t position) const = 0;
  };

  // Шаблонный класс и обычный конструктор.
  template<typename T>
  struct model : concept_t {
    model(const T& x) : data_(move(x)) { }
    void draw_(ostream& out, size_t position) const
    {
      // !!Most important - it's terminal function
      draw(data_, out, position);
    }

    concept_t* copy_() const { return new model(*this); }

    T data_;
  };

  // std::unique_ptr<int_model_t> self_;
  std::unique_ptr<concept_t> self_;
};

using document_t = vector<object_t>;  // полиморфизм только через shared_ptrs

void draw(const document_t&x, ostream &out, size_t position)
{
  out << string(position, ' ') << "<document>" << endl;
  for (const auto& e : x) draw(e, out, position + 2);
  out << string(position, ' ') << "</document>" << endl;
}

// не нужно ничего наследовать.
class my_class_t {

};


void draw(const my_class_t&, ostream& out, size_t position)
{ out << string(position, ' ') << "my_class_t()" << endl; }

}  // namespace

TEST(EvelC11, App) {
  using step5::document_t;
  using step5::my_class_t;

  // TODO:
  document_t document;
  document.reserve(5);

  document.emplace_back(0);
  document.emplace_back(string("hello"));
  document.emplace_back(2);
  document.emplace_back(my_class_t());

  std::reverse(document.begin(), document.end());

  draw(document, cout, 0);

  //object_t a(document);
  //object_t b(my_class_t());
  //a = b;  // not compiled if diff. types
  //b = a;
  // http://en.cppreference.com/w/cpp/language/typeid
  // RTII cost:
  // http://stackoverflow.com/questions/579887/how-expensive-is-rtti
  assert(typeid(document[0]) == typeid(document[1]));
}


namespace ps_sample {
template<typename T>
void draw(const T& x, ostream& out, size_t position)  // object_t -> int and move here
{ out << string(position, ' ') << x << endl; }

class object_t {
public:
  template<typename T>
  object_t(const T& x) : self_(new model<T>(move(x)))  // by value/ специализируем шаблонный класс
  {}

  object_t(const object_t& x) : self_(x.self_->copy_())
  { cout << "copy\n"; }  // если оставить только копирующий констр. компилятор (gcc 4.7) заругается
  object_t(object_t&&) noexcept = default;

  object_t& operator=(const object_t& x)
  { object_t tmp(x); *this = std::move(tmp); return *this; }
  object_t& operator=(object_t&&) noexcept = default;  // Need it!

  friend void draw(const object_t &x, ostream &out, size_t position)
  { x.self_->draw_(out, position); }

private:
  struct concept_t {
    virtual ~concept_t() = default;
    virtual concept_t* copy_() const = 0;
    virtual void draw_(ostream& out, size_t position) const = 0;
  };

  // Шаблонный класс и обычный конструктор.
  template<typename T>
  struct model : concept_t {
    model(const T& x) : data_(move(x)) { }
    void draw_(ostream& out, size_t position) const
    { draw(data_, out, position); }

    concept_t* copy_() const { return new model(*this); }

    T data_;
  };

  // std::unique_ptr<int_model_t> self_;
  std::unique_ptr<concept_t> self_;
};

using document_t = vector<object_t>;  // полиморфизм только через shared_ptrs

void draw(const document_t&x, ostream &out, size_t position)
{
  out << string(position, ' ') << "<document>" << endl;
  for (const auto& e : x) draw(e, out, position + 2);
  out << string(position, ' ') << "</document>" << endl;
}

// не нужно ничего наследовать.
class my_class_t { };

void draw(const my_class_t&, ostream& out, size_t position)
{ out << string(position, ' ') << "my_class_t()" << endl; }

/// Ps
using history_t = vector<document_t>;
void commit(history_t& x) {
  assert(x.size());
  x.push_back(x.back());
}

void undo(history_t& x) { assert(x.size()); x.pop_back(); }

document_t& current(history_t& x) {
  assert(x.size());
  return x.back();
}
}

namespace ps_sample_shared {
// Если большие объекты, но почему так работает? Почему не копий?
template<typename T>
void draw(const T& x, ostream& out, size_t position)  // object_t -> int and move here
{ out << string(position, ' ') << x << endl; }

class object_t {
public:
  template<typename T>
  // make_ptr
  object_t(const T& x) : self_(boost::make_shared<model<T>>(move(x))) {}

  friend void draw(const object_t &x, ostream &out, size_t position)
  { x.self_->draw_(out, position); }

private:
  struct concept_t {
    virtual ~concept_t() = default;
    virtual void draw_(ostream& out, size_t position) const = 0;
  };

  template<typename T>
  struct model : concept_t {
    model(const T& x) : data_(move(x)) { }
    void draw_(ostream& out, size_t position) const
    {
      // а как сделать метод?
      draw(data_, out, position);
    }
    T data_;
  };

  // std::unique_ptr<int_model_t> self_;
  //std::
  boost::shared_ptr<const concept_t> self_;  // ссылки на immutable!! поэтому мы можем возвращаться
  // если подставляет части, то целиком. Это и защищает от копирования.
  // В случае с фотошопом все понятно, иначе пришлось бы копировать все, хотя части были бы неизменными.
  // Но пиксель, например, может быть меньше указателя.
};


using document_t = vector<object_t>;  // полиморфизм только через shared_ptrs

void draw(const document_t&x, ostream &out, size_t position)
{
  out << string(position, ' ') << "<document>" << endl;
  for (const auto& e : x) draw(e, out, position + 2);
  out << string(position, ' ') << "</document>" << endl;
}

// не нужно ничего наследовать.
class my_class_t { };

void draw(const my_class_t&, ostream& out, size_t position)
{ out << string(position, ' ') << "my_class_t()" << endl; }

/// Ps
using history_t = vector<document_t>;
void commit(history_t& x) {
  assert(x.size());
  x.push_back(x.back());
}

void undo(history_t& x) { assert(x.size()); x.pop_back(); }

document_t& current(history_t& x) {
  assert(x.size());
  return x.back();
}

}

TEST(EvelPs, App) {
  using namespace ps_sample_shared;
  history_t h(1);

  // Работаем с верхним элементом
  current(h).emplace_back(0);
  current(h).emplace_back(string("Hello!"));

  draw(current(h), cout, 0);
  cout << "-------------" << endl;

  commit(h);  // сохраняем текущую и ее копируем на верх.

  current(h).emplace_back(current(h));
  current(h).emplace_back(my_class_t());
  current(h)[1] = string("World");

  draw(current(h), cout, 0);
  cout << "-------------" << endl;

  undo(h);
  draw(current(h), cout, 0);

}

// TODO: add features to class

// TODO: Other example, or something
// http://www.codeguru.com/cpp/misc/misc/templatizedclasses/article.php/c14955/Polymorphism-and-TemplateBased-Designs.htm


//TODO: boost::any
// Strange thing.
// http://www.boost.org/doc/libs/1_55_0/doc/html/any.html
// http://stackoverflow.com/questions/9105381/boost-any-get-original-type
// http://sandfly.net.nz/blog/2009/12/the-c-boost-libraries-part-6-boostany/
