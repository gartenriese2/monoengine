.PHONY: all clean distclean


all: build/Makefile
	@make -C build
	@mkdir -p bin
	@cp build/monoEngine bin

build/Makefile: CMakeLists.txt
	@mkdir -p build
	@cd build; cmake ..

clean:
	make -C build clean

distclean:
	rm -rf build imgui.ini contrib/glfw/
