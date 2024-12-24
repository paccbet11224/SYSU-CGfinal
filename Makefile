all: build
	cd build && make -j4

build:
	cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_MAKE_PROGRAM="T:/SYSU/lesson/CG/hw/extern/qt-6.8.0/Tools/MinGW/bin/make.exe"

run: all
	./build/CGfinal

clean:
	rm -rf build/