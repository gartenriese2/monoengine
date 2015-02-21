#include <cstdlib>

#include "demo.hpp"

int main() {

	Demo demo(1920, 1080);
	demo.use(Demo::RenderType::INSTANCEINDEX_CUBE);
	while (demo.render());

	return EXIT_SUCCESS;

}
