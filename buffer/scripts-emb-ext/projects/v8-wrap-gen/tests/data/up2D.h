
// C++
#include <string>

/*

*/
namespace xd {
class New2D : public Hello {
public:
  New2D();  // Находит, но нужно отфильтровать если делать v8::External
  ~New2D();
  void SendUpPower() {

  }

  int SendChannel(const int channel) {

  }

  // state
  // Not Work
  //int currents[2][4];
  int voltages[9];

  // Work
  // GMock cppclean воспринимает только скаляры
  int command;
  //std::vector<String> journal;
 private:
  int hello;
};
}