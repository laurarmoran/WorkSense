.PHONY: build clean go

clean:
	tools/cmake/bin/cmake.exe --build build --target clean

build:
	tools/cmake/bin/cmake.exe --build build
go: build
	build/worksense.exe
