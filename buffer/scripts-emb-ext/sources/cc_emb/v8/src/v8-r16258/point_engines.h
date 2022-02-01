#pragma once

// Other
#include "v8-r16258/include/v8.h"

// App
#include "point_v8.h"

class PointV8Engine {
 public:
   virtual ~PointV8Engine() {}

public:
  virtual void RunWithRealPoint(raw_objects::Point* real_point) = 0;

 protected:
  v8::Isolate* GetIsolate() { return isolate_; }
  v8::Isolate* isolate_;
  virtual v8::Handle<v8::Object> WrapPoint(raw_objects::Point* point) = 0;
};
