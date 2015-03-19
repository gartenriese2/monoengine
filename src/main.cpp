#include <cstdlib>

#include "demo.hpp"
void demo1() {
	Demo demo({1920, 1080});
	while (demo.render());
}

#include "demo2.hpp"
void demo2() {
	Demo2 demo({1920, 1080});
	while (demo.render());
}

int main() {

	demo1();

	return EXIT_SUCCESS;

}
