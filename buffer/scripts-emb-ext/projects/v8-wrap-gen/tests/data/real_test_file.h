#ifndef HELLO_HELLO_
#define HELLO_HELLO_
#define kMaxPABs 10

// Warning:
//   Пока только int!!
//typedef unsigned char uchar;
//typedef unsigned int uint;

class DataBase {
public:
  int locks_[kMaxPABs];
  int lock_events_[kMaxPABs];
  int no_in_powers_[kMaxPABs];
  int no_in_power_events_[kMaxPABs];
  int unlocks_[kMaxPABs];
  int power_is_ok_[kMaxPABs];
};

#endif  // HELLO_HELLO_
