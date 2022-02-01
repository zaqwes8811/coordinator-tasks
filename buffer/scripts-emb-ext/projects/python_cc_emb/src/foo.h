#ifndef FOO_H
#define FOO_H

// C++
#include <string>
#include <iostream>

class Foo{
public:
  Foo() {}
  Foo(std::string const& s) : m_string(s){}
  void doSomething() {
    std::cout << "Foo:" << m_string << std::endl;
  }
  std::string m_string;
};


#endif // FOO_H
