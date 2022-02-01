
// enabling omp in vsee2010 http://www.ssidelnikov.ru/en/2011/05/openmp-visual-studio-2010-express/
#include "app/config.h"

#include <iostream>
#include <gtest/gtest.h>

#ifdef RUN_TESTS_IN_PARALLEL

#ifdef _DEBUG
#undef _DEBUG
#include <omp.h>
#define _DEBUG
#else
#include <omp.h>
#endif
#endif

using namespace std;
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

  omp_set_num_threads( 2 );
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

  omp_set_num_threads( 2 );
  #pragma omp parallel reduction(+:test)
  {
    #pragma omp critical
    cout << "test = " << test << endl;
  }
}