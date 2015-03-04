#include <cstdlib>

#include "demo.hpp"

int main() {

	Demo demo({800, 600});
	while (demo.render());

	return EXIT_SUCCESS;

}
