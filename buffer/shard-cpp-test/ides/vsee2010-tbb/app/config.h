#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_

// enabling omp in vsee2010 http://www.ssidelnikov.ru/en/2011/05/openmp-visual-studio-2010-express/ - fail
// http://software.intel.com/en-us/blogs/2009/04/02/running-your-code-in-parallel-with-tbbparallel_invoke

#define RUN_CONTENT_CASE_IN_PARALLEL

#ifdef RUN_CONTENT_CASE_IN_PARALLEL

//#define OMP_THREADING_FRAMEWORK
#define TBB_THREADING_FRAMEWORK

#endif

#endif 