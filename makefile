CC=gcc
LIB_HEADER= elfd.h
TARGET=libelfd.so
FLAGS= -fstack-protector-all -O2 -fPIC

production:
	make uninstall
	make build_debug
	make install
	make test

build_debug:
	$(CC) source/elfd.c source/elfd_private.c source/elfd_err.c -shared $(FLAGS) -g -o $(TARGET)
	mkdir -p build
	mv $(TARGET) build/$(TARGET)

build:
	$(CC) source/elfd.c source/elfd_internals.c source/elfd_err.c -shared $(FLAGS) -o $(TARGET) 
	mkdir build
	mv $(TARGET) build/$(TARGET)

test:
	$(CC) examples/test.c -o examples/test
	$(CC) examples/main.c -o examples/main -lelfd -g

install:
	sudo cp build/$(TARGET) /usr/lib/$(TARGET)
	sudo cp source/include/elfd.h /usr/include/

uninstall:
	sudo rm -f /usr/lib/$(TARGET)
	sudo rm -f /usr/include/elfd.h
	make clean
clean:
	rm -r -f build