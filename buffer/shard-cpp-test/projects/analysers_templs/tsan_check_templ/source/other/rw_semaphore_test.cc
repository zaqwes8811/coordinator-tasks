// Copyright (c) 2014, Cloudera, inc.

#include <gtest/gtest.h>
#include <boost/foreach.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/locks.hpp>

#include <vector>

//#include "rw_semaphore.h"

using boost::thread;
using std::vector;

struct SharedState {
  SharedState() : int_var(0) {}

  int int_var;
  //rw_semaphore sem;
};

void Writer(SharedState* state) {
  for (int i = 0; i < 10000; i++) {
    //boost::lock_guard<rw_semaphore> l(state->sem);
    state->int_var += i;
  }
}

void Reader(SharedState* state) {

  int prev_val = 0;
  for (int i = 0; i < 10000; i++) {
    //boost::shared_lock<rw_semaphore> l(state->sem);
    // The int var should only be seen to increase.
    //CHECK_GE(state->int_var, prev_val);
    prev_val = state->int_var;
  }
}

TEST(Cloudera, Sem) {
  SharedState s;
  vector<thread*> threads;
  for (int i = 0; i < 10; i++) {
    threads.push_back(new thread(Reader, &s));
  }
  for (int i = 0; i < 2; i++) {
    threads.push_back(new thread(Writer, &s));
  }

  BOOST_FOREACH(thread* t, threads) {
    t->join();
    delete t;
  }
}
