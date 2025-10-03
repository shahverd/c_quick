.PHONY: all configure build clean

all: build

configure:
	@mkdir -p build
	@cd build && cmake ..

	@echo "\033[0;31mYou should run everything from 'build' directory.\033[0m"

build: configure
	@$(MAKE) -C build

run: 
	@clear
	@./build/build/main

clean:
	@rm -rf build

