all:
	mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && make

clean:
	cd build && make clean
purge:
	rm -rf build