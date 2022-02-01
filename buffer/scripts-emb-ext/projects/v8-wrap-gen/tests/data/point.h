
// C++
#include <string>
#include <vector>

class Point {
  public:
   Point(int x, int y) : x_(x),
        y_(y) { }
   int x_;
   int y_;
   //const int* ptr;  // нужно быть внимательным! Var...name  это только int!
   std::string name_;  // Возможно для V8 нужно еще пребразовывать в const char*
   char zorro_;

   // пока считает за скаляры
   std::vector<std::string> vector_values;
   //Vector values_range;
   //int array[42];  // Replace to vector!!
};
