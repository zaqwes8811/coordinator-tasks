// exc, errors and logging - hard coupled
//
#include "top/config.h"

#include <gtest/gtest.h>

//#include <cstdint>  // c++11

//typedef long int  int64_t;

//#define G_USE_BOOST

//
#include "Common/Compat.h"
//#include "Common/TestUtils.h"
//#include "Common/Init.h"
//#include "Common/PageArenaAllocator.h"

//#include <Common/compat-c.h>  // why separeated?
#include <Common/Logger.h>
#include <Common/Error.h>


using namespace ::Hypertable;  // нужно для макросов
TEST(HTTest, Assert) {
  //HT_ASSERT(false);
}
