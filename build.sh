gcc -c debug.c -o build/debug.o &&
gcc -c chunk.c -o build/chunk.o &&
gcc -c vm.c -o build/vm.o &&
gcc -c value.c -o build/value.o &&
gcc -c store.c -o build/store.o &&
gcc -c scanner.c -o build/scanner.o &&
gcc -c compiler.c -o build/compiler.o &&
gcc -c redis.c -o build/redis.o &&
gcc -c main.c -o  build/main.o  &&
gcc build/debug.o \
    build/chunk.o \
    build/vm.o \
    build/value.o \
    build/store.o \
    build/scanner.o \
    build/compiler.o \
    build/redis.o \
    build/main.o \
    -o redis