.PHONY: all clean distclean


COMPILER ?= "clang++"

all: build/Makefile
	@make -j -C build
	@mkdir -p bin
	@cp build/monoEngine bin

build/Makefile: CMakeLists.txt
	@mkdir -p build
	@cd build; cmake -D CMAKE_CXX_COMPILER=$(COMPILER) ..

clean:
	rm -rf build/* bin/*

distclean:
	rm -rf build imgui.ini contrib/glfw/ dl bin
