// Copyright (c) 2013, Cloudera, inc.
#ifndef XXX_UTIL_RW_SEMAPHORE_H
#define XXX_UTIL_RW_SEMAPHORE_H

#include <boost/smart_ptr/detail/yield_k.hpp>
//#include <glog/logging.h>

#include "gutil/atomicops.h"
#include "gutil/dynamic_annotations.h"
#include "gutil/macros.h"

// Read-Write semaphore. 32bit uint that contains the number of readers.
// When someone wants to write, tries to set the 32bit, and waits until
// the readers have finished. Readers are spinning while the write flag is set.
//
// This rw-semaphore makes no attempt at fairness, though it does avoid write
// starvation (no new readers may obtain the lock if a write is waiting).
//
// Given that this is currently based only on spinning (and not futex),
// it should only be used in cases where the lock is held for very short
// time intervals.
//
// If the semaphore is expected to always be released from the same thread
// that acquired it, use rw_spinlock instead.
class rw_semaphore {
 public:
  rw_semaphore() : state_(0) {
  }
  ~rw_semaphore() {}

  void lock_shared() {
    int loop_count = 0;
    Atomic32 cur_state = base::subtle::NoBarrier_Load(&state_);
    while (true) {
      Atomic32 expected = cur_state & kNumReadersMask;   // I expect no write lock
      Atomic32 try_new_state = expected + 1;          // Add me as reader
      cur_state = base::subtle::Acquire_CompareAndSwap(&state_, expected, try_new_state);
      if (cur_state == expected)
        break;
      // Either was already locked by someone else, or CAS failed.
      boost::detail::yield(loop_count++);
    }
  }

  void unlock_shared() {
    int loop_count = 0;
    Atomic32 cur_state = base::subtle::NoBarrier_Load(&state_);
    while (true) {
      DCHECK_GT(cur_state & kNumReadersMask, 0)
        << "unlock_shared() called when there are no shared locks held";
      Atomic32 expected = cur_state;           // I expect a write lock and other readers
      Atomic32 try_new_state = expected - 1;   // Drop me as reader
      cur_state = base::subtle::Release_CompareAndSwap(&state_, expected, try_new_state);
      if (cur_state == expected)
        break;
      // Either was already locked by someone else, or CAS failed.
      boost::detail::yield(loop_count++);
    }
  }

  void lock() {
    int loop_count = 0;
    Atomic32 cur_state = base::subtle::NoBarrier_Load(&state_);
    while (true) {
      Atomic32 expected = cur_state & kNumReadersMask;   // I expect some 0+ readers
      Atomic32 try_new_state = kWriteFlag | expected;    // I want to lock the other writers
      cur_state = base::subtle::Acquire_CompareAndSwap(&state_, expected, try_new_state);
      if (cur_state == expected)
        break;
      // Either was already locked by someone else, or CAS failed.
      boost::detail::yield(loop_count++);
    }

    WaitPendingReaders();
  }

  void unlock() {
    // I expect to be the only writer
    DCHECK_EQ(ANNOTATE_UNPROTECTED_READ(state_), kWriteFlag);
    // reset: no writers/no readers
    Release_Store(&state_, 0);
  }

 private:
  static const uint32_t kNumReadersMask = 0x7fffffff;
  static const uint32_t kWriteFlag = 1 << 31;

  void WaitPendingReaders() {
    int loop_count = 0;
    while ((base::subtle::NoBarrier_Load(&state_) & kNumReadersMask) > 0) {
      boost::detail::yield(loop_count++);
    }
  }

 private:
  volatile Atomic32 state_;
};

#endif /* XXX_UTIL_RW_SEMAPHORE_H */
