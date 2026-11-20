.PHONY: build clean go

CMAKE = cmake 
clean:
	$(CMAKE) --build build --target clean

build:
	$(CMAKE) --build build

go: build
	build/worksense
