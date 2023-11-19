gcc -c debug.c -o build/debug.o &&
gcc -c chunk.c -o build/chunk.o &&
gcc -c vm.c -o build/vm.o &&
gcc -c value.c -o build/value.o &&
gcc -c store.c -o build/store.o &&
gcc -c main.c -o  build/main.o  &&
gcc build/debug.o build/chunk.o build/vm.o build/value.o build/store.o build/main.o -o redis