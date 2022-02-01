// #in.. config.h
#include "process_test_ext.h"

// C++
#include <map>
#include <string>

// Other
#include <gtest/gtest.h>

// App
#include "js_request.h"
#include "v8_wrapper.h"
#include "virtual_machine.h"

using ::scenarios::dblite;
using ::scenarios::V8WrapperImpl;
using ::scenarios::JSRequest;
using std::pair;


TEST(ProcessTestExt, Create) {
  map<string, string> options;
  map<string, string> output;

  // Expose not as object. Can't get keys.
  // Это не полноценный объект JavaScript
  options.insert(pair<string, string>("key", "value"));
  options.insert(pair<string, string>("key1", "value"));
  options.insert(pair<string, string>("key2", "value"));
  options.insert(pair<string, string>("key3", "value"));

	// V8
	int argc = 2;
  const char* argv[] = {"", "./scripts/test_extended.js"};
  v8::V8::InitializeICU();
  string file;
  ParseOptions(argc, argv, options, &file);
  EXPECT_NE(true, file.empty());

  Isolate* isolate = Isolate::GetCurrent();

  // Всегда нужно создать - это как бы свой стек для V8
  HandleScope scope(isolate);

  Handle<String> source = ReadFile(file);
  EXPECT_NE(true, source.IsEmpty());

  // Создаем класс для тестирования
  JsHttpRequestProcessorTestExt processor(isolate, source);

  // Похоже нужно инициализироваться
  processor.Initialize(&options, &output);

  // Не понадобилось. Вообще она как понял создает два объекта в global scope.
  //processor.InstallMapsTest(&
}

TEST(ProcessTestExt, CreateHanedas) {
	dblite* db = new dblite;
	//::scenarios::V8WrapperImpl wrapper(db,"");
	::scenarios::JSRequest wrapper(db);
    wrapper.wrapDataBase("./scripts/test_extended.js");
	//"..\\scripts\\test_extended.js"
	//wrapper.testForExtProcess();

	delete db;
}
