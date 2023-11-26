gcc -c debug.c -o build/debug.o &&
gcc -c chunk.c -o build/chunk.o &&
gcc -c -fPIC vm.c -o build/vm.o &&
gcc -c value.c -o build/value.o &&
gcc -c store.c -o build/store.o &&
gcc -c scanner.c -o build/scanner.o &&
gcc -c -fPIC redis.c -o build/redis.o &&
gcc -c -fPIC compiler.c -o build/compiler.o &&
gcc -shared -o redis.so build/debug.o \
    build/chunk.o \
    build/vm.o \
    build/value.o \
    build/store.o \
    build/scanner.o \
    build/compiler.o \
    build/redis.o
    