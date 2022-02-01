#include "app/config.h"

#include <iostream>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

#ifdef OMP_THREADING_FRAMEWORK

#ifdef _DEBUG
#undef _DEBUG
#include <omp.h>
#define _DEBUG
#else
#include <omp.h>
#endif

#endif


TEST(Parallel, Hello) {
  #pragma omp parallel sections
  {
    #pragma omp section
    {
      //@Thread0
    }

    #pragma omp section
    {
      //@Thread1
    }
  }

  #pragma omp barrier
}

TEST(Parallel, Hello1) {
  EXPECT_EQ(false, true);
}

TEST(Parallel, ConnectionOpenMP) {
  // Must show 2 zero.
  int test( 999 );
#ifdef OMP_THREADING_FRAMEWORK
  omp_set_num_threads( 2 );
#endif
  #pragma omp parallel reduction(+:test)
  {
    #pragma omp critical
    cout << "test = " << test << endl;
  }
}


TEST(Parallel, Hello2) {
  #pragma omp parallel sections
  {
    #pragma omp section
    {
      //@Thread0
    }

    #pragma omp section
    {
      //@Thread1
    }
  } 

  #pragma omp barrier
}

TEST(Parallel, Hello3) {
  EXPECT_EQ(false, true);
}

TEST(Parallel, ConnectionOpenMP2) {
  // Must show 2 zero.
  int test( 999 );
#ifdef OMP_THREADING_FRAMEWORK
  omp_set_num_threads( 2 );
#endif 

  #pragma omp parallel reduction(+:test)
  {
    #pragma omp critical
    cout << "test = " << test << endl;
  }
}

