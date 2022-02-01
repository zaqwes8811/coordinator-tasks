//#include "testLua.h"

using namespace std;
///
int main (int argc, char* argv[])
{
        luaOpenInterpr();
        luaTest();
        ::Sleep(1000);
        luaCloseInterpr();
	      return 0;
}

