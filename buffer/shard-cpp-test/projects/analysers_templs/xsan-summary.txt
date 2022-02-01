CppCon 2014: Kostya Serebryany "Sanitize your C++ code"

ASan:
https://code.google.com/p/address-sanitizer/wiki/AsanCoverage
-O1 -g -fsanitize=address
Some time need options

Cons:
- big failure not founded
- gcc have symbols troubles - FIXME: add -g https://code.google.com/p/thread-sanitizer/wiki/CppManual
http://stackoverflow.com/questions/24145685/how-do-i-initialize-llvms-external-symbolizer

TSan:
https://code.google.com/p/thread-sanitizer/
clang++ -c a.cc -pthread -std=c++11 -fsanitize=thread -g; \
clang++ a.o -o a.out -pthread -std=c++11 -fsanitize=thread; \
./a.out

Troubles:
Boost.thread need build with keys
libc need build with to - how connect this libc to program on compile or runtime
http://stackoverflow.com/questions/20617788/using-memory-sanitizer-with-libstdc
libc and libstdc++ libc++
https://code.google.com/p/thread-sanitizer/issues/detail?id=73
https://code.google.com/p/thread-sanitizer/issues/detail?id=69 - how build?

// FIXME: what it mean? Really
https://code.google.com/p/thread-sanitizer/issues/detail?id=78
Q: My code with C++ exceptions does not work with tsan.
Tsan does not support C++ exceptions.

MSan:
https://code.google.com/p/memory-sanitizer/wiki/MemorySanitizer

Troubles:
https://code.google.com/p/memory-sanitizer/wiki/InstrumentingLibstdcxx

Other tools:
http://google-perftools.googlecode.com/svn/trunk/doc/heap_checker.html
