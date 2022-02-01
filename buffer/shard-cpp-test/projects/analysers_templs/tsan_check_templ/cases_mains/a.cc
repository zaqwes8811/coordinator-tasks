#include <thread>

int main() {
	int X;
	std::thread t([&] { X = 42;});
	X = 43;
	t.join();

	//throw 0;
}