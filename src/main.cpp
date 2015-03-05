#include <cstdlib>

#include "demo.hpp"

int main() {

	Demo demo({1920, 1080});
	while (demo.render());

	return EXIT_SUCCESS;

}
