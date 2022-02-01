#include "tests_config.h"
#include "palette_engine.h"

// Other
#include "v8-r16258/include/v8.h"
#include <gtest/gtest.h>

using raw_objects::Palette;

TEST(V8, Palette) {
  v8::V8::InitializeICU();
  string file = string("./scripts/palette.js");
  ASSERT_NE(true, file.empty());


  /// Isolate and context be injected!
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope handle_scope(isolate);

  // Script
  Handle<String> source = ReadFile(file);
  ASSERT_NE(true, source.IsEmpty());

  ///@Objects
  Palette palette;
  palette.array_[0] = 100;
  palette.point_array[0].x_ = 110;

  // Engine
  PaletteEngine engine(isolate, source, &palette);

  EXPECT_EQ(10, palette.point_array[0].x_);
}
